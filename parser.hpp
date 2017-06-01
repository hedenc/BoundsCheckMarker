/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"

#include <queue>
#include <vector>

#include <initializer_list>
#include <unordered_map>
#include <cstring>

namespace rtboundsmark {

// code node type, either a call or a debug point
enum kind_e { 
    call_node, dbg_node 
};

// Represents a code node in the AST
class node {
public:
    const enum kind_e kind_;

private:
    union {
        char *fname_;
        uint64_t dbg_no_;
    };
public:
    // Constructs a call node
    node(const char *fname): 
        kind_(call_node), fname_(new char [strlen(fname) +1])
    {
        std::strcpy(fname_, fname);
    }
    // Constructs a debug point
    node(uint64_t dbgno): kind_(dbg_node), dbg_no_(dbgno)
    {}

    ~node()
    {
        if (kind_ == call_node && fname_)
            delete [] fname_;
    }

    // I want to avoid implicit deep copies
    explicit node(const node &other);

    node(const node &&other) = delete;

    node &operator = (const node &other) = delete;

    // Prints node to stdout
    void print() const;

    // Getter for debug node, works for debug nodes
    uint64_t dbg_no() const
    {
        return dbg_no_;
    }
};

// Metadata for location data
struct meta_node {
    const uint64_t lineno_, colno_;
    
    // Location lineno:colno
    meta_node(uint64_t lineno, uint64_t colno): 
        lineno_(lineno), colno_(colno) 
    {};

    void print() const
    {
        printf("%llu:%llu", lineno_, colno_);
    }
};
/*
AST for the very simple grammar

program ::= code*

code ::= call_node | debug_node

Debug metadata linenumber info is stored separatly in lineinfo
*/

class ast {
    std::vector<node> code_;
    std::unordered_map<uint64_t, meta_node> lineinfo_;
    friend class parser; /* Gives access to internals for parser */

public:
    void print() const;

    // Getter for lineinfo
    const auto &lineinfo()
    {
        return lineinfo_;
    } 

    // Iterators for program
    auto begin() const
    {
        return code_.begin();
    }

    auto end() const
    {
        return code_.end();
    }
};

/*
Parser
Construct empty AST and pass to parse() to parse
*/
class parser {
    lexer lex_;

    std::queue<token> labuff_; // Look ahead buffer

    bool parse_call(ast &tree);

    bool parse_bang(ast &tree);


    bool expects(std::initializer_list<token> toks);

    bool eat_until(std::initializer_list<token> until);


public:
    /*
    Constructs parser for file with name 'fname'
    */
    parser(const char *fname): 
        lex_(fname), labuff_() 
    {};

    // Parses file and fills up the AST in 'tree'
    void parse(ast &tree);

};

} /* namespace */

#endif