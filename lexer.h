#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <stdbool.h>
#include "symbol_table.h"

typedef enum {
    TOKEN_ID = 2, TOKEN_ASSIGN = 14,
    TOKEN_FUN = 1,TOKEN_NAT = 4,TOKEN_ARROW = 3,TOKEN_QUESTION = 6,
    TOKEN_SEMICOLON = 0,TOKEN_COLON = 7,TOKEN_LITERAL = 10, TOKEN_PLUS = 11,
    TOKEN_BOOL = 5,Token_BrackL = 8, Token_BrackR = 9, Token_and = 12, Token_less = 13
} TokenType;

typedef enum {
    START, INTEGER, IDENTIFIER,DONE
} State;

typedef struct {
    TokenType type;
    char *lexeme;
    int len;
} Token;

typedef struct {
    int line_number;
    State state;
    FILE *reader;
    Token reserved_words[256];
    int reserved_count;
} DFALexer;

#define MAX_CHAR 128

typedef struct {
    State transition[MAX_CHAR];
    bool is_final;
    TokenType token_type;
} DFAState;

// Forward declarations
void reserve(DFALexer *lexer, TokenType type, const char *str);
Token *next_token(DFALexer *lexer,Node **symbolTable);
int read_char(DFALexer *lexer);
void lexer_init(DFALexer *lexer);
void lexer_load_code(DFALexer *lexer, const char *code);
void define_transition(State from, char input, State to);
void define_final_state(State state, TokenType type);
void init_dfa();
bool read_char_equal(DFALexer *lexer, char expected);
void reserve(DFALexer *lexer, TokenType type, const char *str);
void printtoken();

