#pragma once

#include "OutputEngine.h"
#include "hack_map.h"
#include <memory>

class XMLOutputEngine : public OutputEngine {
public:
    XMLOutputEngine (const std::string&, std::shared_ptr<HackMap>, int tab_width = 2);

    void write_keyword (const std::string&);
    void write_identifier (const std::string&);
    void write_symbol (const std::string&);
    void write_int_const (const std::string&);
    void write_string_const (const std::string&);

    void write_non_terminal (const std::string&);
    void close_non_terminal ();

private:
    void write_terminal (const TokenType&, const std::string&);
    void write_spaces ();

private:
    int spaces;
    int tab_width;
};
