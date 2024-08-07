#include "HackSyntaxAnalyzer.h"
#include "OutputEngine.h"
#include "XMLOutputEngine.h"
#include "compilation_engine.h"
#include "error.h"
#include <filesystem>

HackSyntaxAnalyzer::HackSyntaxAnalyzer (std::filesystem::path inp)
: inp (inp), engine (hack_map) {
    if (!std::filesystem::exists (inp))
        throw Error ("Invalid file path");
}


void HackSyntaxAnalyzer::analyze () {
    if (!std::filesystem::is_directory (inp)) {
        if (inp.extension () != ".jack")
            throw Error ("Invalid file extension");
        analyze_file (inp);
        return;
    }

    for (auto& entry : std::filesystem::recursive_directory_iterator (inp)) {
        if (entry.path ().extension () == ".jack")
            analyze_file (entry.path ());
    }
}

std::string HackSyntaxAnalyzer::generate_out_path (std::filesystem::path inp) {
    return inp.replace_extension ("xml");
}

void HackSyntaxAnalyzer::analyze_file (std::filesystem::path inp) {
    std::filesystem::path out (generate_out_path (inp));
    std::cout << "writing to " << out.string () << std::endl;

    std::shared_ptr<Tokenizer> tokenizer = std::make_shared<Tokenizer> (inp, hack_map);
    std::shared_ptr<XMLOutputEngine> output_engine =
    std::make_shared<XMLOutputEngine> (out, hack_map, 1);

    engine.set_output_engine (output_engine);
    engine.set_tokenizer (tokenizer);
    engine.compile ();
}
