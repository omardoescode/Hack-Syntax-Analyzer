
#include "compilation_engine.h"
#include "error.h"
#include <memory>
CompilationEngine::CompilationEngine (std::shared_ptr<HackMap> hack_map,
std::shared_ptr<OutputEngine> output_engine)
: hack_map (hack_map), output_engine (output_engine) {
}

void CompilationEngine::set_tokenizer (std::shared_ptr<Tokenizer> new_tokenizer) {
    tokenizer = new_tokenizer;
}

void CompilationEngine::compile_class () {
    auto token = tokenizer->advance ();
    if (token.value == "class")
        output_engine->write_class ();
    else
        throw Error ("Invalid Class");
}
