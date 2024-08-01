#include "XMLOutputEngine.h"
#include "OutputEngine.h"
#include "error.h"
#include "hack_map.h"
#include <memory>

XMLOutputEngine::XMLOutputEngine (const std::string& out_file_name,
std::shared_ptr<HackMap> hack_map,
int tab_width)
: OutputEngine (out_file_name, hack_map), spaces (0), tab_width (tab_width) {
}

void XMLOutputEngine::write_spaces () {
    out_file << std::string (" ", spaces);
}

void XMLOutputEngine::write_terminal (const TokenType& token, const std::string& value) {
    write_spaces ();

    const std::string& tag = hack_map->get_token (token);
    out_file << "<" << tag << ">" << value << "</" << tag << ">";
}

void XMLOutputEngine::write_keyword (const std::string& keyword) {
    write_terminal (TokenType::KEYWORD, keyword);
}

void XMLOutputEngine::write_identifier (const std::string& identifier) {
    write_terminal (TokenType::IDENTIFIER, identifier);
}
void XMLOutputEngine::write_int_const (const std::string& int_const) {
    write_terminal (TokenType::INT_CONST, int_const);
}
void XMLOutputEngine::write_symbol (const std::string& symbol) {
    write_terminal (TokenType::SYMBOL, symbol);
}
void XMLOutputEngine::write_string_const (const std::string& string_const) {
    write_terminal (TokenType::STRING_CONSTANT, string_const);
}
void XMLOutputEngine::write_non_terminal (const std::string& tag) {
    tags.push (tag);
    write_spaces ();
    out_file << "<" << tag << ">\n";
    spaces += tab_width;
}

void XMLOutputEngine::close_non_terminal () {
    if (tags.empty ())
        throw Error ("Invalid Tags matching");

    auto tag = tags.top ();
    spaces -= tab_width;
    write_spaces ();
    out_file << "</" << tag << ">\n";
}
