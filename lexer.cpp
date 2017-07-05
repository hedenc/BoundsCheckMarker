#include "lexer.hpp"
#include <cinttypes>

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

const std::unordered_map<std::string, token> lexer::keywords_ = {
    {"", eof}
#define X(str, sym) ,{ #str, sym}
#include "keywords.def"
#undef X
};

// Moves first non eaten character to 'head_'
void lexer::read()
{
    if (!head_set_){
        head_ = fgetc(input_);
        head_set_ = 1;
    }
}

token lexer::state_slash()
{
    do {
        read();
        switch (head_) {
        case '\n':
        case EOF:
            return other;
        case ':':
            eat();
            return lineinfo;
        default:
            val_ += head_;
            eat();
            break;
        }
    } while (true);
}

token lexer::state_alpha()
{

    do {
        read();
        switch (head_) {
#define X(symbol) case symbol:
#include "gzGZ_.def"
#include "afAF.def"
X('0')
#include "19.def"
#undef X
            val_ += head_,
            eat();
            break;
        default:
            //printf("%d strval() = %s\n", other, val_.c_str());
            auto search = keywords_.find(val_);
            return (search != keywords_.end()) ? search->second : id;
        }
    } while (true);
}

token lexer::next_tok()
{
    do {
        read();
        switch (head_) {
        case ' ':
        case '\t':
        case '\n':
            eat();
            break;
        case '/':
            val_ = head_;
            eat();
            return state_slash();
        case '<':
            eat();
            return langle;
        case '>':
            eat();
            return rangle;
        case '-':
            val_ = head_;
            eat();
            return state_minus();
        case '0':
            val_ = head_;
            eat();
            return state_zero();
#define X(number) case number:
#include "19.def"
#undef X
            val_ = head_;
            eat();
            return state_num();
#define X(letter) case letter:
#include "afAF.def"
#include "gzGZ_.def"
#undef X
            val_ = head_;
            eat();
            return state_alpha();

        case EOF:
            return eof;
        default:
            val_ = head_;
            eat();
            return other;
        }

    } while (1);
}

token lexer::state_num()
{
    do {
        read();
        switch (head_) {
#define X(num) case num:
X('0')
#include "19.def"
#include "afAF.def"
#undef X
            val_ += head_;
            eat();
            break;
        case ':':
            eat();
            return addr;
        default:
            return num;
        }
    } while (true);
}

token lexer::state_zero()
{
    read();
    switch (head_) {
#define X(num) case num:
X('0')
#include "19.def"
#include "afAF.def"
#undef X
        val_ += head_;
        eat();
        return state_num();
    case 'x':
    case 'X':
        val_ += head_;
        eat();
        return state_num();
    default:
        return num;
    }
}

token lexer::state_minus()
{
    read();
    switch (head_) {
    case '0':
        val_ += head_;
        eat();
        return state_zero();
#define X(num) case num:
#include "19.def"
#include "afAF.def"
#undef X
        val_ += head_;
        eat();
        return state_num();
    default:
        eat();
        return other;
    }
}

void lexer::print()
{
    token ctok;
    do {
        ctok = next_tok();
        switch (ctok){
        case lineinfo:
            printf("lineinfo[%s]\n", strval());
            break;
        case other:
            printf("other[%s]\n", strval());
            break;
        case addr:
            printf("addr[%" PRIx64 "]\n", hexval());
            break;
        case id:
            printf("id[%s]\n", strval());
            break;
        case num:
            printf("num[%s]\n", strval());
            break;

/*
Generates blocks for each token with following code
            printf("sym\n");
            break;
*/
#define X(sym) case sym: printf(#sym "\n"); break;
X(eof)
X(langle)
X(rangle)
#undef X
#define X(str, sym) case sym: printf(# sym "\n"); break;
#include "keywords.def"
#undef X
        }
    } while (ctok != eof);
}

} /* namespace */


