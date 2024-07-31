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

    std::string out = std::filesystem::is_directory (inp) ?
    inp.stem ().string () + ".xml" :
    inp.stem ().string ().substr (0, inp.stem ().string ().length () - 5) + ".xml";

    std::cout << "Writing to " << out << std::endl;
}
