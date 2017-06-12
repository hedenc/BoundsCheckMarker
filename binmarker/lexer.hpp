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
    other, eof, addr, comma, lparen, rparen, intlit, reg, intconst, proc, id
};

class lexer {
    FILE *input_;
    bool head_set_;         // true if 'head_' is set with non handled character
    int head_;    
    std::string val_;
    size_t start_addr;

    void read();
    
    void eat()              // Eats character in 'head_'
    {
        head_set_ = false;
    }

    token state1();
    token state2();
    token state3();
    token state4();
    token state5();
    token state6();
    token state7();
    token state8();

    token return_addr();

    //const static std::unordered_map<std::string, token> keywords_;

public:
    lexer(FILE *file): // Constructs lexer for opened file 'file'
        input_(file), head_set_(false), val_(), start_addr(0)
    {}

    // Returns token data for tokens that use string token data
    const char *strval() const
    {
        return val_.c_str();
    }

    // Returns token data for tokens that use hexformated integer token data
    uint64_t hexval() const
    {
        size_t pos;
        return std::stoull(val_, &pos, 16);
    }

    // Returns token data for tokens that use integer token data
    int64_t intval() const
    {
        return std::stoll(val_);
    }

    // Advances the lexer and returns next token
    token next_tok();

    // Prints all the tokens in the file to stdout
    void print();
};

} /* namespace */

#endif //LEXER_HPP