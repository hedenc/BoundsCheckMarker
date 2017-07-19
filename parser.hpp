#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include "code_block.hpp"

#include <unordered_map>
#include <vector>
#include <map>
#include <unordered_set>
#include <map>
#include <set>
#include <array>


/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#ifdef __APPLE__
#define RELJMPADDRS
#endif

namespace binmark {

using std::vector;
using std::map;
using std::string;
using std::array;
using std::set;
using std::unordered_set;
using std::unordered_map;

/*
Parser
parse() to parse
*/

class ast: public map<
    string, // filename
    map< 
        uint64_t, // Line number
        array< set< uint64_t >, 0 + // Addresses
#define X(KINDNUM, FNAME, DESC, FROM, TO) + 2
#include "functions.def"
#undef X
        >
    >
> {
public:
    void print() const;
};


class parser {
    template <token... tok>
    struct helper {
        static constexpr bool expect(parser &);
    };

    template <token... tok_e>
    friend struct helper;

    lexer lex_;

    token ctok_;
    bool ctok_set_;

    size_t calls_;


#ifdef RELJMPADDRS
    int64_t jump_offset_;
#else
    uint64_t jump_addr_;
#endif

    map<uint64_t, unordered_set<uint64_t> > split_block_addr_;

    const static unordered_map<string, uint64_t> funcs_;

    linedata curr_line_;
    bool after_ret_;

    void eat()
    {
        ctok_set_ = false;
    }

    template <token... tok>
    constexpr bool expects() {
        return helper<tok...>::expect(*this);
    }

    void parse_addr(ast &tree, vector<code_block> &block);
    void parse_lineinfo();
    void read();
    void handle_jump(vector<code_block> &blocks, uint64_t instr, uint64_t split_addr, code_block &from);



public:
    /*
    Constructs parser for opened file 'file'
    */
    parser(FILE *file):
        lex_(file), ctok_(eof), ctok_set_(false), calls_(0), 
#ifdef RELJMPADDRS
        jump_offset_(0), 
#else
        jump_addr_(0),
#endif
        curr_line_("???", 0), after_ret_(false)
    {};

    // Parses up till next call node found, sets 'nxtnode' to that
    void parse(ast &tree, vector<code_block> &blocks);
    //void print() const;

};

template <>
struct parser::helper<> {
    static constexpr bool expect(parser &p) {
        return true;
    }
};

template <token head, token... tail>
struct parser::helper<head, tail...> {
    static constexpr bool expect(parser &p) {
        p.read();
        if (p.ctok_ != head)
            return false;
        p.eat();
        return helper<tail...>::expect(p);
    }
};

} /* namespace */

#endif // PARSER_HPP