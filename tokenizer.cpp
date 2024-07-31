#include "tokenizer.h"
#include "enums.h"
#include "error.h"
#include "hack_map.h"
#include <algorithm>
#include <regex>
#include <sstream>
#include <string>

Tokenizer::Tokenizer (std::filesystem::path inp_file, std::shared_ptr<HackMap> hack_map)
: inp_file (inp_file), in_string{ false }, in_comment_block{ false },
  hack_map{ hack_map }, current{ -1 } {
    tokenize ();
}

bool Tokenizer::has_more_tokens () {
    return (current + 1) < tokens_vec.size ();
}

token Tokenizer::advance () {
    current++;
    return tokens_vec[current];
}

void Tokenizer::tokenize () {
    std::string current_line;
    while (std::getline (inp_file, current_line)) {
        if (inp_file.bad ())
            throw Error ("Tokenizer::tokenize: Error reading input file");

        trim_codeline (current_line);

        if (current_line.empty ())
            continue;

        std::istringstream codeline_stream{ current_line };
        std::string value;
        while (codeline_stream >> value)
            process_word (value);
    }
}

void Tokenizer::trim_codeline (std::string& line) {
    // Remove Comment
    auto comment = line.find ("//");
    if (comment != std::string::npos)
        line.erase (comment);

    // Remove leading and trailing spaces
    std::regex pattern (R"(^\s+|\s+$)");
    line = std::regex_replace (line, pattern, "");
}

void Tokenizer::process_word (const std::string& word) {
    std::string token;
    for (int i = 0, __end = word.length (); i < __end; i++) {
        auto& c = word[i];

        // handle strings
        if (!in_comment_block && in_string) {
            if (c == '"') {
                process_value (token, true);
                in_string = false;
            } else {
                token += c;
            }
        } else if (!in_comment_block && c == '"') {
            if (!token.empty ())
                process_value (token);
            token     = "";
            in_string = true;
            // handle comments
        } else if (in_comment_block) {
            if (word.substr (i, 2) == "*/") {
                i++;
                in_comment_block = false;
            }
        } else if (word.substr (i, 2) == "/*") {
            i++;
            in_comment_block = true;
            // handle standalone symbols
        } else if (hack_map->contains_symbol (c)) {
            if (!token.empty ())
                process_value (token);
            token = "";
            std::string value (1, c);
            process_value (value);
            // otherwise, keep adding to the token
        } else {
            token += c;
        }
    }

    if (!token.empty ())
        process_value (token);
}

void Tokenizer::process_value (const std::string& value, bool is_string) {
    if (is_string)
        tokens_vec.push_back ({ value, TokenType::STRING_CONSTANT });
    else if (hack_map->contains_keyword (value))
        tokens_vec.push_back ({ value, TokenType::KEYWORD });
    else if (std::all_of (value.begin (), value.end (), ::isdigit))
        tokens_vec.push_back ({ value, TokenType::INT_CONST });
    else if (value.length () == 1 && hack_map->contains_symbol (value[0]))
        tokens_vec.push_back ({ value, TokenType::SYMBOL });
    else
        tokens_vec.push_back ({ value, TokenType::IDENTIFIER });
}
