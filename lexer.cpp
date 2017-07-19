#include "lexer.hpp"
#include <cinttypes>

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

decltype(lexer::keywords_) lexer::keywords_ = {
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
        //fprintf(stderr, "    %c\n", head_);
    }
}

// Called upon match on "/"
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

// Called upon match on [a-zA-Z_] 
token lexer::state_alpha()
{

    do {
        read();
        switch (head_) {
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '0' ... '9':
        case '_':
            val_ += head_,
            eat();
            break;
        default:
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
        case '1' ... '9':
            val_ = head_;
            eat();
            return state_num();
        case 'a' ... 'z':
        case 'A' ... 'Z':
        case '_':
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

// Called upon match of  [1-9] | ( "0" ( [0-9a-fA-F] | "x" ) )
token lexer::state_num()
{
    do {
        read();
        switch (head_) {
        case '0' ... '9':
        case 'a' ... 'z':
        case 'A' ... 'Z':
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

// Called upon match on "0"
token lexer::state_zero()
{
    read();
    switch (head_) {
    case '0' ... '9':
    case 'a' ... 'f':
    case 'A' ... 'F':
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

// Match -
token lexer::state_minus()
{
    read();
    switch (head_) {
    case '0':
        val_ += head_;
        eat();
        return state_zero();
    case '1' ... '9':
    case 'a' ... 'f':
    case 'A' ... 'F':
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
            printf("lineinfo[%s]\n", val_.c_str());
            break;
        case other:
            printf("other[%s]\n", val_.c_str());
            break;
        case addr:
            printf("addr[%" PRIx64 "]\n", hexval());
            break;
        case id:
            printf("id[%s]\n", val_.c_str());
            break;
        case num:
            printf("num[%s]\n", val_.c_str());
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
X(callq)
X(jump)
X(ret)
X(jmp)
#undef X
/*
#define X(str, sym) case sym: printf(# sym "\n"); break;
#include "keywords.def"
#undef X
*/
        }
    } while (ctok != eof);
}

} /* namespace */


