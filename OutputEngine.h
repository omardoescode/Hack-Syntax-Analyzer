#pragma once
#include "enums.h"
#include "hack_map.h"
#include <fstream>
#include <memory>
#include <stack>
#include <string>

class OutputEngine {
public:
    OutputEngine (const std::string&, std::shared_ptr<HackMap> hack_map);

    virtual void write_keyword (const std::string&);
    virtual void write_identifier (const std::string&);
    virtual void write_symbol (const std::string&);
    virtual void write_int_const (const std::string&);
    virtual void write_string_const (const std::string&);

    virtual void write_non_terminal (const std::string&);
    virtual void close_non_terminal ();

private:
    virtual void write_terminal (const TokenType&, const std::string&);


protected:
    std::ofstream out_file;
    std::shared_ptr<HackMap> hack_map;
    std::stack<std::string> tags;
};
