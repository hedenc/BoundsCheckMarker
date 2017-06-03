/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#include "parser.hpp"

#include <cstdio>
#include <stdexcept>

#define STR(X) #X

namespace rtboundsmark {

node::node(const node &other): 
    kind_(other.kind_)
{
    switch (kind_) {
    case call_node:
        fname_ = other.fname_;
        break;
    case dbg_node:
        dbg_no_ = other.dbg_no_;
        break;
    }
}

void node::print() const
{
    switch (kind_) {
    case call_node:
        printf("%s", fname_);
        break;
    case dbg_node:
        printf("!%llu", dbg_no_);
        break;
    }
}

void ast::print() const
{
    for (auto &c: code_)
        c.print();
    for (auto &l: lineinfo_)
        printf(
            "%llu = meta %llu:%llu\n", 
            l.first, l.second.lineno_, l.second.colno_
            );
}

void parser::parse(ast &tree) 
{
    do {
        if (labuff_.empty()) {
            labuff_.push(lex_.next_tok());
        }
        switch (labuff_.front()) {
        case call:
            if (parse_call(tree))
                continue;
            break;
        case bang:
            if (parse_bang(tree))
                continue;
            break;
        case eof:
            labuff_.pop();
            return;
        default:
            ;
        }
        labuff_.pop();
    } while (true);
}

// Parses when matching [!]
bool parser::parse_bang(ast &tree)
{
    uint64_t metano, lineno, colno;
    labuff_.push(lex_.next_tok());
    switch (labuff_.back()) {
    case dbg:
        if (!expects({bang, intlit}))
            break;
        tree.code_.emplace_back(lex_.intval());
        labuff_ = {};
        return true;
    case intlit:
        metano = lex_.intval();
        if (! expects({eqsign, meta, bang, lbrace, i32, intlit}))
            break;
        lineno = lex_.intval();
        if (! expects({comma, i32, intlit}))
            break;
        colno = lex_.intval();
        if (! expects({comma, meta, bang, intlit, comma}))
            break;
        labuff_.push(lex_.next_tok());
        switch (labuff_.back()) {
        case meta:
            if (! expects({bang, intlit})) 
                break;
            /* fall through */
        case null:
            if (!expects({rbrace}))
                break;
            tree.lineinfo_.emplace(metano, meta_node(lineno, colno));
            labuff_ = {};
            return true;
        default:
            ;
        }
    default:
        ;
    }
    return false;
}

// Parses when matching [call]
bool parser::parse_call(ast &tree)
{
    if (!eat_until({at}) || !expects({func}))
        return false;

    char *strnode = new char [strlen(lex_.strval()) +1];
    strcpy(strnode, lex_.strval());
    tree.code_.emplace_back(strnode);

    if (!expects({lparen}) || !eat_until({rparen}))
        throw std::runtime_error(
            "Expected () after function " __FILE__ ":" STR(__LINE__)
        );

    labuff_ = {};
    return true;
}

/*
Takes a list of tokens that are expected in order by the parser
Returns true if match
*/
bool parser::expects(std::initializer_list<token> toks)
{
    for (auto it : toks) {
        labuff_.push(lex_.next_tok());
        if (labuff_.back() != it)
            return false;
    }
    return true;
}

/*
Consumes tokens until any of the argument tokens are found
Returns false if EOF is found
*/
bool parser::eat_until(std::initializer_list<token> until)
{
    do {
        labuff_.push(lex_.next_tok());
        if (labuff_.back() == eof)
            break;
        for (auto it : until) {
            if (labuff_.back() == it)
                return true;
        }
    } while (true);
    return false;
}

} /* namespace */


