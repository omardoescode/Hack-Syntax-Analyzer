#include "hack_map.h"
#include "error.h"


std::string HackMap::get_keyword (Keyword value) {
    return keywords[value];
}

std::string HackMap::get_token (TokenType value) {
    return tokens[value];
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
    for (auto& val : symbols)
        if (val == test)
            return true;
    return false;
}

std::map<Keyword, std::string> HackMap::keywords = {
    { Keyword::CLASS, "class" },
    { Keyword::METHOD, "method" },
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
    { TokenType::INT_CONST, "int constant" },
    { TokenType::STRING_CONSTANT, "string constant" },
};

std::vector<char> HackMap::symbols = { '(', ')', '{', '}', '[', ']', ',', ';',
    '+', '-', '*', '/', '&', '|', '<', '>', '=', '~' };
