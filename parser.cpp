#include "parser.hpp"
#include <cstring>

#include <cinttypes>
/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

const std::unordered_map<std::string, const char * > parser::funcs_({
    {"", ""}
#define X(FNAME, DESC) , { #FNAME , # DESC }
#include "functions.def"
#undef X
});

void parser::parse_addr(std::multimap<fline, std::string> &ast)
{
    uint64_t instr = lex_.hexval();
    if (expects<callq, num, langle, id>()) {
        auto search = funcs_.find(lex_.strval());
        if (search != funcs_.end()) {
            cline_.addr_ = instr;
            ast.emplace(cline_, search->second);
        }
    }
}

void parser::parse_lineinfo()
{
    static std::string buff;

    buff.assign(lex_.strval());
    if (expects<num>())
        cline_ = fline(buff.c_str(), lex_.intval());
}

void parser::parse(std::multimap<fline, std::string> &ast)
{
    do {
        ctok_ = lex_.next_tok();
        switch (ctok_) {
        case lineinfo:
            parse_lineinfo();
            break;
        case addr:
            parse_addr(ast);
            break;
        default:
            ;
        }
    } while (ctok_ != eof);
}



} /* namespace */
