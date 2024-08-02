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
    bool contains_keyword_constant (std::string);
    bool contains_symbol (char);
    bool contains_operator (char);

private:
    template <typename T> bool contains (T value, std::vector<T> values);

private:
    static std::map<Keyword, std::string> keywords;
    static std::vector<std::string> keyword_constants;
    static std::map<TokenType, std::string> tokens; // This is just used for debugging
    static std::vector<char> symbols, operators;
};
