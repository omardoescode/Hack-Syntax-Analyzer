#include "HackSyntaxAnalyzer.h"
#include <filesystem>
#include <iostream>

int main (int argc, char* argv[]) {
    if (argc != 2) {
        std::cout << "Invalid number of argument" << std::endl;
        return 0;
    }

    std::filesystem::path inp = argv[1];
    if (!std::filesystem::exists (inp)) {
        std::cout << "Invalid file path" << std::endl;
        return 1;
    }

    HackSyntaxAnalyzer analyzer (inp);
    analyzer.analyze ();
}
