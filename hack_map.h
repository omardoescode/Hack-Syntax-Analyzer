#pragma once
#include "enums.h"
#include <map>
#include <string>
#include <vector>
class HackMap {
public:
    std::string get_keyword (Keyword);
    bool contains_keyword (std::string);
    bool contains_symbol (char);

private:
    static std::map<Keyword, std::string> keywords;
    static std::vector<char> symbols;
};
