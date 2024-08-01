#pragma once

#include "OutputEngine.h"
#include "enums.h"
#include "hack_map.h"
#include <memory>

class XMLOutputEngine : public OutputEngine {
public:
    XMLOutputEngine (const std::string&, std::shared_ptr<HackMap>, int tab_width = 2);

    void write_terminal (Token);
    void write_non_terminal (const std::string&);
    void close_non_terminal ();

private:
    void write_spaces ();

private:
    int spaces;
    int tab_width;
};
