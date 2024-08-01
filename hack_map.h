#pragma once
#include "enums.h"
#include <map>
#include <string>
#include <vector>
class HackMap {
public:
    std::string get_keyword (Keyword);
    Keyword get_keyword (const std::string&);
    std::string get_token (TokenType);
    bool contains_keyword (std::string);
    bool contains_symbol (char);
    bool contains_operator (char);

private:
    static std::map<Keyword, std::string> keywords;
    static std::map<TokenType, std::string> tokens; // This is just used for debugging
    static std::vector<char> symbols, operators;
};
