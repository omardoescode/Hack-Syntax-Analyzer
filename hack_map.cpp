#include "hack_map.h"


std::string HackMap::get_keyword (Keyword value) {
    return keywords[value];
}
std::map<Keyword, std::string> HackMap::keywords = {
    { Keyword::CLASS, "class" },
    { Keyword::METHOD, "method" },
    { Keyword::CONSTRUCTOR, "constructor" },
    { Keyword::INT, "int" },
    { Keyword::BOOL, "BOOL" },
    { Keyword::CHAR, "CHAR" },
    { Keyword::VOID, "VOID" },
    { Keyword::VAR, "VAR" },
    { Keyword::STATIC, "STATIC" },
    { Keyword::FIELD, "FIELD" },
    { Keyword::LET, "LET" },
    { Keyword::DO, "DO" },
    { Keyword::IF, "IF" },
    { Keyword::ELSE, "ELSE" },
    { Keyword::WHILE, "WHILE" },
    { Keyword::RETURN, "RETURN" },
    { Keyword::TRUE, "TRUE" },
    { Keyword::FALSE, "FALSE" },
    { Keyword::THIS, "this" },
    { Keyword::NULL_keyword, "null" },
};