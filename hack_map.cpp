#include "hack_map.h"
#include "error.h"


std::string HackMap::get_keyword (Keyword value) {
    return keywords[value];
}

std::string HackMap::get_token (TokenType value) {
    return tokens[value];
}

template <typename T> bool HackMap::contains (T target, std::vector<T> values) {
    for (auto& val : values)
        if (val == target)
            return true;
    return false;
}
Keyword HackMap::get_keyword (const std::string& target) {
    for (auto& [key, value] : keywords)
        if (value == target)
            return key;
    throw Error ("HackMap::get_keyword: Keyword not found");
}

bool HackMap::contains_keyword (std::string target) {
    for (auto& [key, value] : keywords)
        if (value == target)
            return true;
    return false;
}

bool HackMap::contains_symbol (char test) {
    return contains (test, symbols);
}

bool HackMap::contains_operator (char test) {
    return contains (test, operators);
}

bool HackMap::contains_keyword_constant (std::string target) {
    return contains (target, keyword_constants);
}
std::map<Keyword, std::string> HackMap::keywords = {
    { Keyword::CLASS, "class" },
    { Keyword::METHOD, "method" },
    { Keyword::FUNCTION, "function" },
    { Keyword::CONSTRUCTOR, "constructor" },
    { Keyword::INT, "int" },
    { Keyword::BOOL, "bool" },
    { Keyword::CHAR, "char" },
    { Keyword::VOID, "void" },
    { Keyword::VAR, "var" },
    { Keyword::STATIC, "static" },
    { Keyword::FIELD, "field" },
    { Keyword::LET, "let" },
    { Keyword::DO, "do" },
    { Keyword::IF, "if" },
    { Keyword::ELSE, "else" },
    { Keyword::WHILE, "while" },
    { Keyword::RETURN, "return" },
    { Keyword::TRUE, "true" },
    { Keyword::FALSE, "false" },
    { Keyword::THIS, "this" },
    { Keyword::NULL_keyword, "null" },
};

std::map<TokenType, std::string> HackMap::tokens = {
    { TokenType::SYMBOL, "symbol" },
    { TokenType::IDENTIFIER, "identifier" },
    { TokenType::KEYWORD, "keyword" },
    { TokenType::INT_CONST, "int_constant" },
    { TokenType::STRING_CONSTANT, "string_constant" },
};

std::vector<char> HackMap::symbols = { '(', ')', '{', '}', '[', ']', ',', ';',
    '.', '+', '-', '*', '/', '&', '|', '<', '>', '=', '~' };

std::vector<char> HackMap::operators = { '+', '-', '*', '/', '&', '|', '<', '>', '~' };

std::vector<std::string> HackMap::keyword_constants = { "true", "false", "null", "this" };
