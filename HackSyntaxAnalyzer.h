#pragma once

#include "compilation_engine.h"
#include "hack_map.h"
#include <filesystem>

class HackSyntaxAnalyzer {
public:
    HackSyntaxAnalyzer (std::filesystem::path inp);

    void analyze ();

private:
    void analyze_file (std::filesystem::path);
    std::string generate_out_path (std::filesystem::path);

private:
    std::filesystem::path inp;
    std::shared_ptr<HackMap> hack_map;
    CompilationEngine engine;
};
