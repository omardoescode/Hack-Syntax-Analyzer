#include "XMLOutputEngine.h"
#include "OutputEngine.h"
#include "enums.h"
#include "error.h"
#include "hack_map.h"
#include <memory>

XMLOutputEngine::XMLOutputEngine (const std::string& out_file_name,
std::shared_ptr<HackMap> hack_map,
int tab_width)
: OutputEngine (out_file_name, hack_map), spaces (0), tab_width (tab_width) {
}

void XMLOutputEngine::write_spaces () {
    int temp = spaces;
    while (temp--)
        out_file << " ";
}

void XMLOutputEngine::write_terminal (Token token) {
    write_spaces ();

    const std::string& tag = hack_map->get_token (token.type);
    out_file << "<" << tag << ">" << token.value << "</" << tag << ">\n";
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
    tags.pop ();

    spaces -= tab_width;
    write_spaces ();
    out_file << "</" << tag << ">\n";
}
