#include "parser.hpp"

#include <cinttypes>
#include <stack>
/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

using std::stack;

decltype(parser::funcs_) parser::funcs_({
    {"", 0}
#define X(KINDNUM, FNAME, DESC, FROM, TO) , { #FNAME , 2 * KINDNUM }
#include "functions.def"
#undef X
});

void parser::read()
{
    if (!ctok_set_)
        ctok_ = lex_.next_tok();
    ctok_set_ = true;
}

void parser::handle_jump(vector<code_block> &blocks, uint64_t instr, uint64_t split_addr, code_block &from)
{
    if (split_addr < instr) {
        for (auto it = blocks.rbegin(); it != blocks.rend(); 
            ++it) {
            if (it->startinsno() == split_addr) {
                from.doublelink(*it, split_addr);
                break;
            } else if (it->startinsno() < split_addr) {
                auto inserted_elem = blocks.emplace(it.base(), *it);

                inserted_elem->handle_split(split_addr, from);
                break;
            }
        }
    } else if (split_addr > instr) {
        from.link(split_addr);
        split_block_addr_[split_addr].insert(from.startinsno());
    }
}


void parser::parse_addr(ast &tree, vector<code_block> &blocks)
{
    uint64_t instr = lex_.hexval();

    code_block new_block(instr, curr_line_);

    bool insert_new_block = false;

    if (blocks.empty())
        blocks.emplace_back(new_block);
    else {
#ifdef RELJMPADDRS
        if (jump_offset_) {
            handle_jump(blocks, instr, instr + jump_offset_, blocks.back());
            jump_offset_ = 0;
            insert_new_block = true;
        }
#endif
        if (blocks.empty() || 
            !blocks.back().same_source(curr_line_)) {
            insert_new_block = true;
        }
        if (!split_block_addr_.empty()) {
            auto range = split_block_addr_.begin();
            if (range->first == instr) {
                for (auto a: range->second)
                    new_block.backlink(a);
                split_block_addr_.erase(range);
                insert_new_block = true;
            }
        }
        if (after_ret_) {
            after_ret_ = false;
            blocks.emplace_back(new_block);
        } else if (insert_new_block) {
            new_block.backlink(blocks.back().startinsno());
            blocks.back().link(new_block.startinsno());
            blocks.emplace_back(new_block);
        }
    }
    read();
    switch (ctok_) {
    case callq:
        eat();
        if (expects<num, langle, id>()) {
            eat();
            auto search = funcs_.find(lex_.strval());
            if (search != funcs_.end()) {
                ++calls_;
                //printf("found func call\n");
                tree[blocks.back().source()][blocks.back().lineno()]
                    [search->second].insert(instr)
                ;
            }
        }
        break;
    case jump:
        eat();
        if (expects<num>()) {
            eat();
#ifdef RELJMPADDRS
            jump_offset_ = lex_.intval();
#else
            handle_jump(blocks, instr, lex_.hexval(), blocks.back());
#endif
        }
        break;
    case jmp:
        eat();
        if (expects<num>()) {
            eat();
            after_ret_ = true;
#ifdef RELJMPADDRS
            jump_offset_ = lex_.intval();
#else
            handle_jump(blocks, instr, lex_.hexval(), blocks.back());
#endif
        }
        break;
    case ret:
        eat();
        after_ret_ = true;
        break;
    default:
        break;
    }
}

void parser::parse_lineinfo()
{
    static string buff;

    buff.assign(lex_.strval());
    eat();
    if (expects<num>()) {
        eat();
        curr_line_ = linedata(buff, lex_.intval());
    }
}

void parser::parse(ast &tree, vector<code_block> &blocks)
{
    do {
        read();
        switch (ctok_) {
        case lineinfo:
            eat();
            parse_lineinfo();
            break;
        case addr:
            eat();
            parse_addr(tree, blocks);
            break;
        default:
            eat();
        }
    } while (ctok_ != eof);

    printf("Number of check calls: %zu\n", calls_);
}

void ast::print() const
{
    size_t checked_lines = 0;
    const char *info[] = {
#define X(KINDNUM, FNAME, DESC, FROM, TO) # DESC , "Inlined " # DESC, 
#include "functions.def"
#undef X
        ""
    };

    for (auto &f: *this) {
        printf("%s\n", f.first.c_str());
        for (auto &l: f.second) {
            printf("Line %4" PRIu64 "  ", l.first);
            ++checked_lines;
            size_t i = 0;
            while (l.second[i].empty())
                ++i;
            printf("%-24s at ", info[i]);
            for (auto a: l.second[i])
                printf("%" PRIx64 " ", a);
            printf("\n");
            ++i;
            while (i < l.second.size()) {
                if (!l.second[i].empty()) {
                    printf("           %-24s at ", info[i]);
                    for (auto a: l.second[i])
                        printf("%" PRIx64 " ", a);
                    printf("\n");
                }
                ++i;
            }
        }
        printf("\n           Checked lines: %zu\n\n", checked_lines);
        checked_lines = 0;
    }
}


} /* namespace */
