#ifndef LEXER_HPP
#define LEXER_HPP
#include <string>
#include <unordered_map>

/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

namespace binmark {

enum token {
    other, eof, lineinfo, addr, id, langle, rangle, num
#define X(str, sym) ,sym
#include "keywords.def"
#undef X
};

class lexer {
    FILE *input_;
    bool head_set_;         // true if 'head_' is set with non handled character
    int head_;

    std::string val_;

    const static std::unordered_map<std::string, token> keywords_;

    void read();
    
    void eat()              // Eats character in 'head_'
    {
        head_set_ = false;
    }



    token state_slash();
    token state_alpha();
   
    token state_langle();
    token state_langle_alpha();

    token state_num();
    token state_zero();
    token state_minus();

public:
    lexer(FILE *file): // Constructs lexer for opened file 'file'
        input_(file), head_set_(false), val_() //, start_addr(0)
    {}

    // Returns token data for tokens that use string token data
    const char *strval() const
    {
        return val_.c_str();
    }

    // Returns token data for tokens that use integer token data
    int64_t intval() const
    {
        size_t pos;
        return std::stoull(val_, &pos, 0);
    }

    // Returns token data for tokens that use hexformated integer token data
    uint64_t hexval() const
    {
        //fprintf(stderr, "%s\n", strval());
        size_t pos;
        return std::stoull(val_, &pos, 16);
    }

    // Advances the lexer and returns next token
    token next_tok();

    // Prints all the tokens in the file to stdout
    void print();
};

} /* namespace */

#endif //LEXER_HPP
