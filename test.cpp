
#include "hack_map.h"
#include "tokenizer.h"
#include <filesystem>
#include <iostream>

bool test_hack_map ();
bool test_tokenizer ();
bool test (bool (*func) (), std::string);

int main () {
    return test (test_hack_map, std::string ("Testing HackMap")) &&
    test (test_tokenizer, std::string ("Testing Tokenizer"));
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
