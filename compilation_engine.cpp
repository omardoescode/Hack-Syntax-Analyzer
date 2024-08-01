
#include "compilation_engine.h"
#include "enums.h"
#include "error.h"
#include "token.h"
#include <memory>
#include <regex>
CompilationEngine::CompilationEngine (std::shared_ptr<HackMap> hack_map,
std::shared_ptr<OutputEngine> output_engine,
std::shared_ptr<Tokenizer> tokenizer)
: hack_map (hack_map), output_engine (output_engine), tokenizer (tokenizer) {
    compile_class ();
}

void CompilationEngine::set_tokenizer (std::shared_ptr<Tokenizer> new_tokenizer) {
    tokenizer = new_tokenizer;
}

void CompilationEngine::compile_class () {
    output_engine->write_non_terminal ("class");
    Token token;
    token = tokenizer->advance ();
    output_engine->write_keyword (token.value);

    token = tokenizer->advance ();
    output_engine->write_identifier (token.value);

    token = tokenizer->advance ();
    output_engine->write_symbol (token.value);

    compile_class_var_dec ();
    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_class_var_dec () {
    Token token;
    token = tokenizer->next ();
    std::regex pattern ("\\b(static|field)\\b");
    while (std::regex_match (token.value, pattern)) {
        output_engine->write_non_terminal ("ClassVarDec");

        token = tokenizer->advance ();
        output_engine->write_keyword (token.value);

        _compile_dec ();

        token = tokenizer->next ();
        std::regex pattern (".*;.*");
        while (!std::regex_match (token.value, pattern)) {
            token = tokenizer->advance ();
            output_engine->write_symbol (token.value);

            _compile_dec ();

            token = tokenizer->advance ();
        }

        output_engine->write_symbol (token.value);

        output_engine->close_non_terminal ();
    }
}

void CompilationEngine::_compile_dec () {
    Token token;
    token = tokenizer->advance ();
    output_engine->write_keyword (token.value);

    token = tokenizer->advance ();
    output_engine->write_identifier (token.value);
}
