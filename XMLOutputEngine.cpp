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
    while ((temp--) != 0) {
        out_file << " ";
    }
}

void XMLOutputEngine::write_terminal (Token token) {
    write_spaces ();

    const std::string& tag = hack_map->get_token (token.type);
    out_file << "<" << tag << "> " << token.value << " </" << tag << ">\n";
}

void XMLOutputEngine::write_non_terminal (const NonTerminalRules& tag) {
    tags.push (tag);
    write_spaces ();
    out_file << "<" << non_terminals[tag] << ">\n";
    spaces += tab_width;
}

void XMLOutputEngine::close_non_terminal () {
    if (tags.empty ()) {
        throw Error ("Invalid Tags matching");
    }

    auto tag = tags.top ();
    tags.pop ();

    spaces -= tab_width;
    write_spaces ();
    out_file << "</" << non_terminals[tag] << ">\n";
}

std::map<NonTerminalRules, std::string> XMLOutputEngine::non_terminals = {
    { NonTerminalRules::CLASS, "class" },
    { NonTerminalRules::CLASS_VAR_DEC, "ClassVarDec" },
    { NonTerminalRules::SUBROUTINE_DEC, "SubroutineDec" },
    { NonTerminalRules::SUBROUTINE_BODY, "SubroutineBody" },
    { NonTerminalRules::VAR_DEC, "VarDec" },
    { NonTerminalRules::VAR_DEC, "Statements" },
    { NonTerminalRules::IF_STATEMENT, "IfStatement" },
    { NonTerminalRules::LET_STATEMENT, "Let_STATEMENT" },
    { NonTerminalRules::WHILE_STATEMENT, "WhileStatement" },
    { NonTerminalRules::RETURN_STATEMENT, "ReturnStatement" },
    { NonTerminalRules::DO_STATEMENT, "DoStatement" },
    { NonTerminalRules::PARAMETER_LIST, "ParameterLilst" },
    { NonTerminalRules::EXPRESSION_LIST, "ExpressionList" },
    { NonTerminalRules::EXPRESSION, "Expression" },
    { NonTerminalRules::TERM, "Term" },
};
