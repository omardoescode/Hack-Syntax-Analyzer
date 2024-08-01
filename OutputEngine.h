#pragma once
#include "enums.h"
#include "hack_map.h"
#include "token.h"
#include <fstream>
#include <memory>
#include <stack>
#include <string>

class OutputEngine {
public:
    OutputEngine (const std::string&, std::shared_ptr<HackMap> hack_map);

    virtual void write_terminal (Token)                  = 0;
    virtual void write_non_terminal (const std::string&) = 0;
    virtual void close_non_terminal ()                   = 0;

private:
protected:
    std::ofstream out_file;
    std::shared_ptr<HackMap> hack_map;
    std::stack<std::string> tags;
};
