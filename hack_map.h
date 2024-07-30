#pragma once
#include "enums.h"
#include <map>
#include <string>
class HackMap {
public:
    std::string get_keyword (Keyword keyword);

private:
    static std::map<Keyword, std::string> keywords;
};
