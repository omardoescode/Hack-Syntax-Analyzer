#pragma once

enum class TokenType {
    KEYWORD,
    SYMBOL,
    INT_CONST,
    STRING_CONSTANT,
    IDENTIFIER
};

enum class Keyword {
    CLASS,
    METHOD,
    CONSTRUCTOR,
    FUNCTION,
    INT,
    BOOL,
    CHAR,
    VOID,
    VAR,
    STATIC,
    FIELD,
    LET,
    DO,
    IF,
    ELSE,
    WHILE,
    RETURN,
    TRUE,
    FALSE,
    NULL_keyword,
    THIS,
};
