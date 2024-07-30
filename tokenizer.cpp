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
  hack_map{ hack_map } {
}

bool Tokenizer::has_more_tokens () {
    return !tokens_queue.empty () || !inp_file.eof ();
}

token Tokenizer::advance () {
    if (!tokens_queue.empty ()) {
        auto value = tokens_queue.front ();
        tokens_queue.pop ();
        return value;
    }

    std::string current_line;
    while (true) {
        std::getline (inp_file, current_line);

        if (inp_file.bad ())
            throw Error ("Parser::readFile: Error reading file");
        trim_codeline (current_line);

        if (!current_line.empty ())
            break;
    }

    std::istringstream codeline_stream{ current_line };
    std::string value;
    while (codeline_stream >> value)
        process_word (value);

    auto result = tokens_queue.front ();
    tokens_queue.pop ();
    return result;
}

void Tokenizer::trim_codeline (std::string& line) {
    // Remove leading and trailing spaces
    std::regex pattern (R"(^\s+|\s+$)");
    line = std::regex_replace (line, pattern, "");

    // Remove Comment
    auto comment = line.find ("//");
    if (comment != std::string::npos)
        line.erase (comment);
}

void Tokenizer::process_word (const std::string& word) {
    std::string token;
    for (int i = 0, __end = word.length (); i < __end; i++) {
        auto& c = word[i];

        if (in_string) {
            if (c == '"')
                process_value (token, true), in_string = false;
            else
                token += c;
        } else if (c == '"') {
            process_value (token), token = "", in_string = true;
        } else if (in_comment_block) {
            if (word.substr (i, 2) == "*/")
                in_comment_block = false;
        } else if (word.substr (i, 2) == "/*") {
            in_comment_block = true;
        } else if (hack_map->contains_symbol (c)) {
            if (!token.empty ())
                process_value (token), token = "";
            process_value (std::to_string (c));
        } else {
            token += c;
        }
    }

    process_value (token);
}

void Tokenizer::process_value (const std::string& value, bool is_string) {
    if (is_string)
        tokens_queue.push ({ value, TokenType::STRING_CONSTANT });
    else if (hack_map->contains_keyword (value))
        tokens_queue.push ({ value, TokenType::KEYWORD });
    else if (std::all_of (value.begin (), value.end (), ::isdigit))
        tokens_queue.push ({ value, TokenType::INT_CONST });
    else if (value.length () == 1 && hack_map->contains_symbol (value[0]))
        tokens_queue.push ({ value, TokenType::SYMBOL });
    else
        tokens_queue.push ({ value, TokenType::IDENTIFIER });
}
