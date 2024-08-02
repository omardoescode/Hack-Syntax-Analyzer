#include "XMLOutputEngine.h"
#include "compilation_engine.h"
#include "error.h"
#include "hack_map.h"
#include "tokenizer.h"
#include <filesystem>
#include <iostream>
#include <memory>
#include <regex>

bool test_hack_map ();
bool test_tokenizer ();
bool test_compilation_engine_and_xml_output_engine ();
bool test (bool (*func) (), std::string);

int main () {
    test (test_hack_map, std::string ("Testing HackMap"));
    test (test_tokenizer, std::string ("Testing Tokenizer"));
    test (test_compilation_engine_and_xml_output_engine,
    "Testing the compilation engine and the xml output engine");
}

bool test (bool (*func) (), std::string test_name) {
    if (func ()) {
        std::cout << "-- " << test_name << " Succeced" << std::endl;
        return true;
    }

    std::cout << "-- " << test_name << " Failed" << std::endl;
    return false;
}
bool test_hack_map () {
    HackMap hack_map;
    return hack_map.contains_keyword ("class") && hack_map.contains_symbol ('*');
}

bool test_tokenizer () {
    std::shared_ptr<HackMap> hack_map;
    std::filesystem::path inp_file ("tests/test.jack");

    if (!std::filesystem::exists (inp_file)) {
        std::cout << "--- file not found" << std::endl;
        return false;
    }

    Tokenizer tokenizer (inp_file, hack_map);
    while (tokenizer.has_more_tokens ()) {
        auto token = tokenizer.advance ();
        std::cout << token.value << " " << hack_map->get_token (token.type) << std::endl;
    }
    return true;
}
bool test_compilation_engine_and_xml_output_engine () {

    std::filesystem::path inp_file ("tests/test.jack");

    if (!std::filesystem::exists (inp_file)) {
        std::cout << "--- file not found" << std::endl;
        return false;
    }

    try {
        std::shared_ptr<HackMap> hack_map;
        std::shared_ptr<Tokenizer> tokenizer =
        std::make_shared<Tokenizer> (inp_file, hack_map);
        std::shared_ptr<XMLOutputEngine> output_engine =
        std::make_shared<XMLOutputEngine> ("tests/test.xml", hack_map, 1);
        CompilationEngine engine (hack_map);
        engine.set_tokenizer (tokenizer);
        engine.set_output_engine (output_engine);
        engine.compile ();
        return true;
    } catch (Error& errr) {
        // throw errr;
        return false;
    }
}
