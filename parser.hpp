#ifndef PARSER_HPP
#define PARSER_HPP

#include "lexer.hpp"
#include <unordered_map>
#include <map>
#include <functional>
#include <cstring>

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

struct fline {
    std::string fname_;
    uint64_t lineno_;

    uint64_t addr_;

    fline(): fname_("???"), lineno_(0){};
    fline(const char *fname, uint64_t lineno):
        fname_(fname), lineno_(lineno) {};

    bool operator<(const fline &other) const {
        return (fname_ != other.fname_) ?
            fname_ < other.fname_: lineno_ < other.lineno_;
    }
};


/*
Parser
parse() to parse
*/

struct str_cmp {
    bool operator()(const char *s1, const char *s2) const
    {
        return !strcmp(s1, s2);
    };
};


class parser {


    lexer lex_;

    token ctok_;

    void parse_addr(std::multimap<fline, std::string> &ast);
    void parse_lineinfo();

    const static std::unordered_map<std::string, const char * > funcs_;

    fline cline_;

    template <token... tok>
    struct helper {
        static constexpr bool expect(parser &);
    };

    template <token... tok_e>
    friend struct helper;

    template <token... tok>
    constexpr bool expects() {
        return helper<tok...>::expect(*this);
    }

public:
    /*
    Constructs parser for opened file 'file'
    */
    parser(FILE *file):
        lex_(file), ctok_(eof), cline_()
    {};

    // Parses up till next call node found, sets 'nxtnode' to that
    void parse(std::multimap<fline, std::string> &ast);

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
        p.ctok_ = p.lex_.next_tok();
        if (p.ctok_ != head)
            return false;
        return helper<tail...>::expect(p);
    }
};

} /* namespace */

#endif // PARSER_HPP