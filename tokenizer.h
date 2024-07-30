#pragma once
#include "enums.h"
#include "hack_map.h"
#include "token.h"
#include <filesystem>
#include <fstream>
#include <memory>
#include <queue>
#include <string>
class Tokenizer {
public:
    Tokenizer (std::filesystem::path, std::shared_ptr<HackMap>);

    bool has_more_tokens ();
    token advance ();
    TokenType token_type ();
    Keyword keyword ();
    char symbol ();
    std::string identifier ();
    std::string string_val ();
    int int_val ();

private:
    void trim_codeline (std::string&);
    void process_word (const std::string&);
    void process_value (const std::string&, bool is_string = false);

private:
    std::queue<token> tokens_queue;
    std::ifstream inp_file;
    bool in_comment_block, in_string;
    std::shared_ptr<HackMap> hack_map;
};
