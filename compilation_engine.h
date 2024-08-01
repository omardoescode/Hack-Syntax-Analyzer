#pragma once

#include "OutputEngine.h"
#include "hack_map.h"
#include "tokenizer.h"
class CompilationEngine {
public:
    CompilationEngine (std::shared_ptr<HackMap>, std::shared_ptr<OutputEngine>);

    // Setters
    void set_tokenizer (std::shared_ptr<Tokenizer>);
    void compile ();

private:
    // Compilers
    void compile_class ();
    void compile_class_var_dec ();
    void compile_subroutine_dec ();
    void compile_parameter_list ();
    void compile_subroutine_body ();
    void compile_var_dec ();
    void compile_let ();
    void compile_if ();
    void compile_while ();
    void compile_return ();
    void compile_do ();
    void compile_expression ();
    void compile_term ();
    void compile_expression_list ();
    void compile_statements ();

    // Helpers
    void advance_and_write ();
    void _compile_dec ();
    void _compile_multiple_dec ();
    void _compile_var_name ();

private:
    std::shared_ptr<Tokenizer> tokenizer;
    std::shared_ptr<HackMap> hack_map;
    std::shared_ptr<OutputEngine> output_engine;
};
