#pragma once
#include "enums.h"
#include "hack_map.h"
#include "token.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <string>
#include <vector>
class Tokenizer {
public:
    Tokenizer (std::filesystem::path, std::shared_ptr<HackMap>);

    bool has_more_tokens ();
    token advance ();
    TokenType token_type ();
    Keyword keyword ();
    int int_const ();
    std::string string_const ();
    std::string identifier ();
    char symbol ();

private:
    void tokenize ();
    void trim_codeline (std::string&);
    void process_word (const std::string&);
    void process_value (const std::string&, bool is_string = false);
    void check_current_token (TokenType, std::string);

private:
    std::vector<token> tokens_vec;
    std::ifstream inp_file;
    bool in_comment_block, in_string;
    std::shared_ptr<HackMap> hack_map;
    int current = -1;
};
