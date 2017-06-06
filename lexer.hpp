/*
Author: hedenc@kth.se
Free to use and modify non commercially as long as this notice remains
*/

#ifndef LEXER_HPP
#define LEXER_HPP

#include <unordered_map>
#include <string>

namespace rtboundsmark {

/*
Tokens, other matches irrelevant token that isn't whitespace
*/
enum token {
    other, eof, intlit, comma, at, call, 
    func, i32, lparen, rparen
};
/*
Lexer class, gives next token with next_tok() last token is eof.
To fetch token data use intval() or strval() observe the string returned from
strval() has to be copied or from Lexer.
*/
class lexer {
    FILE *input_;           // Input file
    bool head_set_;         // true if 'head_' is set with non handled character
    int head_;              // Reading head
    std::string val_;       // Stores token data

    void read();
    
    void eat()              // Eats character in 'head_'
    {
        head_set_ = false;
    }

    token state1();  

    token state2();

    const static std::unordered_map<std::string, token> keywords_;

public:
    // Constructs lexer for file named 'fname'
    lexer(const char *fname);

    // Returns token data for tokens that use string token data
    const char *strval() const
    {
        return val_.c_str();
    }

    // Returns token data for tokens that use integer token data
    uint64_t intval() const
    { 
        return std::stoull(val_);
    }

    // Advances the lexer and returns next token
    token next_tok();

    // Prints all the tokens in the file to stdout
    void print();
};

} /* namespace */

#endif