
#include "compilation_engine.h"
#include "enums.h"
#include "token.h"
#include <memory>
#include <regex>
CompilationEngine::CompilationEngine (std::shared_ptr<HackMap> hack_map,
std::shared_ptr<OutputEngine> output_engine)
: hack_map (hack_map), output_engine (output_engine) {
}

void CompilationEngine::set_tokenizer (std::shared_ptr<Tokenizer> new_tokenizer) {
    tokenizer = new_tokenizer;
}

void CompilationEngine::compile () {
    compile_class ();
}

void CompilationEngine::advance_and_write () {
    Token token = tokenizer->advance ();
    output_engine->write_terminal (token);
}

void CompilationEngine::compile_class () {
    output_engine->write_non_terminal ("class");
    Token token;
    advance_and_write ();

    advance_and_write ();

    advance_and_write ();

    compile_class_var_dec ();

    compile_subroutine_dec ();

    output_engine->close_non_terminal ();
}

void CompilationEngine::compile_class_var_dec () {
    Token token;
    token = tokenizer->next ();
    std::regex pattern ("\\b(static|field)\\b");

    while (std::regex_match (token.value, pattern)) {
        output_engine->write_non_terminal ("ClassVarDec");

        advance_and_write ();

        _compile_multiple_dec ();

        advance_and_write ();

        output_engine->close_non_terminal ();

        token = tokenizer->next ();
    }
}


void CompilationEngine::compile_subroutine_dec () {
    Token token;

    token = tokenizer->next ();
    std::regex pattern ("\\b(constructor|method|function)\\b");

    while (std::regex_match (token.value, pattern)) {

        output_engine->write_non_terminal ("SubroutineDec");

        // constructor|method|function
        advance_and_write ();

        // (void|type)
        advance_and_write ();

        // subroutine name
        advance_and_write ();

        // (
        advance_and_write ();

        // parameter_list
        compile_parameter_list ();

        // )
        advance_and_write ();

        // {
        advance_and_write ();

        // subroutine body
        compile_subroutine_body ();

        // }
        advance_and_write ();
        token = tokenizer->next ();
        output_engine->close_non_terminal ();
    }
}

void CompilationEngine::compile_subroutine_body () {
    output_engine->write_non_terminal ("SubroutineBody");
    compile_var_dec ();
    compile_statements ();
    output_engine->close_non_terminal ();
}
void CompilationEngine::compile_var_dec () {
    Token token;

    std::regex pattern ("\\b(var)\\b");
    token = tokenizer->next ();
    while (std::regex_match (token.value, pattern)) {
        output_engine->write_non_terminal ("VarDec");
        // var
        advance_and_write ();

        // name
        advance_and_write ();

        std::regex pattern (",");
        token = tokenizer->next ();
        while (std::regex_match (token.value, pattern)) {
            // name
            advance_and_write ();
        }

        // ;
        advance_and_write ();

        output_engine->close_non_terminal ();

        token = tokenizer->next ();
    }
}

void CompilationEngine::compile_statements () {
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
}

void CompilationEngine::compile_let () {
    // let
    advance_and_write ();

    // varName
    advance_and_write ();

    // WARN: Check for the possiblity for brackets

    // =
    advance_and_write ();

    // =
    compile_expression ();
}

void CompilationEngine::compile_parameter_list () {
    Token token;
    token = tokenizer->next ();
    if (token.value != ")") {
        _compile_multiple_dec ();
    }
}

void CompilationEngine::compile_expression_list () {
}
void CompilationEngine::compile_expression () {
    Token token;

    compile_term ();

    token = tokenizer->next ();
    if (token.type == TokenType::SYMBOL &&
    hack_map->contains_operator (token.value.at (0))) {
        // contains_operator
        advance_and_write ();

        compile_term ();
    }
}

void CompilationEngine::compile_term () {
    Token token;

    token = tokenizer->next ();
    if (token.type == TokenType::IDENTIFIER) {
        _compile_var_name ();
    } else if (token.type == TokenType::SYMBOL) {
        // ~|-
        advance_and_write ();

        compile_term ();
    } else {
        advance_and_write ();
    }
}

void CompilationEngine::_compile_dec () {
    Token token;

    // Type
    advance_and_write ();

    advance_and_write ();
}

void CompilationEngine::_compile_multiple_dec () {
    Token token;
    _compile_dec ();

    token = tokenizer->next ();
    std::regex pattern (",");
    while (std::regex_match (token.value, pattern)) {
        advance_and_write ();

        _compile_dec ();

        token = tokenizer->next ();
    }
}
