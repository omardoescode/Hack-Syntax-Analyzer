#include "compilation_engine.h"
#include "OutputEngine.h"
#include "enums.h"
#include "error.h"
#include "token.h"
#include <iterator>
#include <memory>
#include <regex>
#include <string>
CompilationEngine::CompilationEngine (std::shared_ptr<HackMap> hack_map)
: hack_map (hack_map) {
}

void CompilationEngine::set_output_engine (std::shared_ptr<OutputEngine> new_output_engine) {
    output_engine = new_output_engine;
}
void CompilationEngine::set_tokenizer (std::shared_ptr<Tokenizer> new_tokenizer) {
    tokenizer = new_tokenizer;
}

void CompilationEngine::compile () {
    compile_class ();
}

void CompilationEngine::advance_and_write (TokenType type) {
    Token token = tokenizer->advance ();

    if (token.type != type)
        throw Error ("Invalid token type");
    output_engine->write_terminal (token);
}

void CompilationEngine::advance_and_write (std::vector<TokenType> types) {
    Token token = tokenizer->advance ();

    for (auto& type : types) {
        if (type == token.type) {
            output_engine->write_terminal (token);
            return;
        }
    }

    throw Error ("Invalid type");
}
void CompilationEngine::advance_and_write (TokenType type, std::string value) {
    Token token = tokenizer->advance ();

    if (token.type != type) {
        throw Error ("Invalid token type");
    }
    std::regex pattern (value);
    if (!std::regex_match (token.value, pattern))
        throw Error ("Invalid token value");

    output_engine->write_terminal (token);
}

void CompilationEngine::compile_class () {
    output_engine->write_non_terminal (NonTerminalRules::CLASS);
    Token token;
    advance_and_write (TokenType::KEYWORD, "\\bclass\\b");

    advance_and_write (TokenType::IDENTIFIER);

    advance_and_write (TokenType::SYMBOL, "\\{");

    compile_class_var_dec ();

    compile_subroutine_dec ();

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_class_var_dec () {
    Token token;
    token = tokenizer->next ();
    std::regex pattern ("\\b(static|field)\\b");

    while (std::regex_match (token.value, pattern)) {
        output_engine->write_non_terminal (NonTerminalRules::CLASS_VAR_DEC);

        // static|field
        advance_and_write (TokenType::KEYWORD);

        _compile_multiple_dec ();

        advance_and_write (TokenType::SYMBOL, ";");

        output_engine->close_non_terminal ();

        token = tokenizer->next ();
    }
}


void CompilationEngine::compile_subroutine_dec () {
    Token token;

    token = tokenizer->next ();
    std::regex pattern ("\\b(constructor|method|function)\\b");

    while (std::regex_match (token.value, pattern)) {

        output_engine->write_non_terminal (NonTerminalRules::SUBROUTINE_DEC);

        // constructor|method|function
        advance_and_write (TokenType::KEYWORD, "\\b(constructor|method|function)\\b");

        // (void|type)
        advance_and_write ({ TokenType::IDENTIFIER, TokenType::KEYWORD });

        // subroutine name
        advance_and_write (TokenType::IDENTIFIER);

        // (
        advance_and_write (TokenType::SYMBOL, "\\(");

        // parameter_list
        compile_parameter_list ();

        // )
        advance_and_write (TokenType::SYMBOL, "\\)");


        // subroutine body
        compile_subroutine_body ();

        token = tokenizer->next ();
        output_engine->close_non_terminal ();
    }
}

void CompilationEngine::compile_subroutine_body () {
    output_engine->write_non_terminal (NonTerminalRules::SUBROUTINE_BODY);

    // {
    advance_and_write (TokenType::SYMBOL, "\\{");

    compile_var_dec ();
    compile_statements ();

    // }
    advance_and_write (TokenType::SYMBOL, "\\}");

    output_engine->close_non_terminal ();
}
void CompilationEngine::compile_var_dec () {
    Token token;

    std::regex pattern ("\\b(var)\\b");
    token = tokenizer->next ();

    while (std::regex_match (token.value, pattern)) {
        output_engine->write_non_terminal (NonTerminalRules::VAR_DEC);
        // var
        advance_and_write (TokenType::KEYWORD, "\\bvar\\b");

        // type
        advance_and_write ({ TokenType::KEYWORD, TokenType::IDENTIFIER });

        // name
        advance_and_write (TokenType::IDENTIFIER);

        std::regex pattern (",");
        token = tokenizer->next ();
        while (std::regex_match (token.value, pattern)) {
            // ,
            advance_and_write (TokenType::SYMBOL, ",");

            // name
            advance_and_write (TokenType::IDENTIFIER);

            // either , or ;
            token = tokenizer->next ();
        }

        // ;
        advance_and_write (TokenType::SYMBOL, ";");

        output_engine->close_non_terminal ();

        token = tokenizer->next ();
    }
}

void CompilationEngine::compile_statements () {
    output_engine->write_non_terminal (NonTerminalRules::STATEMENTS);
    Token token;
    std::regex pattern ("\\b(let|do|return|while|if)\\b");

    token = tokenizer->next ();
    while (std::regex_match (token.value, pattern)) {
        if (token.value == "let")
            compile_let ();
        else if (token.value == "if")
            compile_if ();
        else if (token.value == "return")
            compile_return ();
        else if (token.value == "do")
            compile_do ();
        else if (token.value == "while")
            compile_while ();

        token = tokenizer->next ();
    }
    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_let () {
    output_engine->write_non_terminal (NonTerminalRules::LET_STATEMENT);

    Token token;
    // let
    advance_and_write (TokenType::KEYWORD, "let");

    // varName
    advance_and_write (TokenType::IDENTIFIER);

    token = tokenizer->next ();
    if (token.value == "[") {
        advance_and_write (TokenType::SYMBOL, "\\[");
        compile_expression ();
        advance_and_write (TokenType::SYMBOL, "\\]");
    }

    // =
    advance_and_write (TokenType::SYMBOL, "=");

    // expression
    compile_expression ();

    advance_and_write (TokenType::SYMBOL, ";");

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_if () {
    output_engine->write_non_terminal (NonTerminalRules::IF_STATEMENT);

    Token token;
    advance_and_write (TokenType::KEYWORD, "if");

    advance_and_write (TokenType::SYMBOL, "\\(");

    compile_expression ();

    advance_and_write (TokenType::SYMBOL, "\\)");

    advance_and_write (TokenType::SYMBOL, "\\{");

    compile_statements ();

    advance_and_write (TokenType::SYMBOL, "\\}");

    token = tokenizer->next ();

    if (token.value == "else") {
        advance_and_write (TokenType::KEYWORD, "else");

        advance_and_write (TokenType::SYMBOL, "\\{");

        compile_statements ();

        advance_and_write (TokenType::SYMBOL, "\\}");
    }

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_while () {
    output_engine->write_non_terminal (NonTerminalRules::WHILE_STATEMENT);

    advance_and_write (TokenType::KEYWORD, "while");

    advance_and_write (TokenType::SYMBOL, "\\(");

    compile_expression ();

    advance_and_write (TokenType::SYMBOL, "\\)");

    advance_and_write (TokenType::SYMBOL, "\\{");

    compile_statements ();

    advance_and_write (TokenType::SYMBOL, "\\}");

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_do () {
    output_engine->write_non_terminal (NonTerminalRules::DO_STATEMENT);

    advance_and_write (TokenType::KEYWORD, "do");

    _compile_subroutine_call ();

    advance_and_write (TokenType::SYMBOL, ";");

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_return () {
    output_engine->write_non_terminal (NonTerminalRules::RETURN_STATEMENT);

    Token token;

    advance_and_write (TokenType::KEYWORD, "return");

    token = tokenizer->next ();
    if (token.value != ";")
        compile_expression ();

    advance_and_write (TokenType::SYMBOL, ";");

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_parameter_list () {
    output_engine->write_non_terminal (NonTerminalRules::PARAMETER_LIST);

    Token token;
    token = tokenizer->next ();
    while (token.value != ")") {
        advance_and_write ({ TokenType::IDENTIFIER, TokenType::KEYWORD });
        advance_and_write (TokenType::IDENTIFIER);

        token = tokenizer->next ();

        if (token.value == ",")
            advance_and_write (TokenType::SYMBOL, ",");
    }

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_expression_list (std::string end) {
    output_engine->write_non_terminal (NonTerminalRules::EXPRESSION_LIST);

    Token token = tokenizer->next ();
    std::regex pattern (end);
    while (!std::regex_match (token.value, pattern)) {
        compile_expression ();

        token = tokenizer->next ();
        if (token.value == ",") {
            advance_and_write (TokenType::SYMBOL, "\\,");
        }

        token = tokenizer->next ();
    }
    output_engine->close_non_terminal ();
}
void CompilationEngine::compile_expression () {
    output_engine->write_non_terminal (NonTerminalRules::EXPRESSION);
    Token token;

    compile_term ();

    token = tokenizer->next ();
    if (token.type == TokenType::SYMBOL &&
    hack_map->contains_operator (token.value.at (0))) {
        // operators
        advance_and_write (TokenType::SYMBOL);

        compile_expression ();
    }

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_term () {
    output_engine->write_non_terminal (NonTerminalRules::TERM);

    Token token;

    token = tokenizer->next ();

    switch (token.type) {
    case TokenType::INT_CONST: advance_and_write (TokenType::INT_CONST); break;
    case TokenType::STRING_CONST:
        advance_and_write (TokenType::STRING_CONST);
        break;
    case TokenType::KEYWORD:
        if (!hack_map->contains_keyword_constant (token.value))
            throw Error ("Invalid keyword");

        advance_and_write (TokenType::KEYWORD);

        break;
    case TokenType::SYMBOL:
        if (token.value == "-" || token.value == "~") {
            advance_and_write (TokenType::SYMBOL);
            compile_expression ();
        } else if (token.value == "(") {
            advance_and_write (TokenType::SYMBOL, "\\(");
            compile_expression ();
            advance_and_write (TokenType::SYMBOL, "\\)");
        }
        break;
    case TokenType::IDENTIFIER: {
        _compile_subroutine_call (false);
    }
    }

    output_engine->close_non_terminal ();
}


void CompilationEngine::_compile_multiple_dec () {

    Token token;

    advance_and_write ({ TokenType::KEYWORD, TokenType::IDENTIFIER });

    advance_and_write (TokenType::IDENTIFIER);

    token = tokenizer->next ();
    std::regex pattern (",");
    while (std::regex_match (token.value, pattern)) {
        // ,
        advance_and_write (TokenType::SYMBOL, ",");

        advance_and_write (TokenType::IDENTIFIER);

        token = tokenizer->next ();
    }
}

void CompilationEngine::_compile_subroutine_call (bool must) {
    Token token;

    advance_and_write (TokenType::IDENTIFIER);
    token = tokenizer->next ();
    if (token.value == "[") {
        advance_and_write (TokenType::SYMBOL, "\\[");
        compile_expression ();
        advance_and_write (TokenType::SYMBOL, "\\]");
    } else if (token.value == "(") {
        advance_and_write (TokenType::SYMBOL, "\\(");
        compile_expression_list ("\\)");
        advance_and_write (TokenType::SYMBOL, "\\)");
    } else if (token.value == ".") {
        while (token.value == ".") {
            advance_and_write (TokenType::SYMBOL, "\\.");
            advance_and_write (TokenType::IDENTIFIER);

            token = tokenizer->next ();
        }
        advance_and_write (TokenType::SYMBOL, "\\(");
        compile_expression_list ("\\)");
        advance_and_write (TokenType::SYMBOL, "\\)");
    } else if (must) {
        throw Error ("Invalid Subroutine Name");
    }
}
