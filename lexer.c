#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <stdbool.h>
#include "lexer.h"

// Define the transition table and the number of states
#define NUM_STATES (DONE + 1)
DFAState transition_table[NUM_STATES];

void define_transition(State from, char input, State to) {
    transition_table[from].transition[(int)input] = to;
}

void define_final_state(State state, TokenType type) {
    transition_table[state].is_final = true;
    transition_table[state].token_type = type;
}

// Function to initialize the lexer
void lexer_init(DFALexer *lexer) {
    lexer->line_number = 1;
    lexer->state = START;
    lexer->reserved_count = 0;
    lexer->reader = NULL;
    // Reserve some words
    reserve(lexer, TOKEN_FUN, "fun");
    reserve(lexer, TOKEN_NAT, "nat");
    reserve(lexer,TOKEN_BOOL,"bool");
}

// Function to load code into the lexer
void lexer_load_code(DFALexer *lexer, const char *filename) {
    if (lexer->reader) {
        fclose(lexer->reader);
    }
    lexer->reader = fopen(filename, "r");
    if (!lexer->reader) {
        perror("Error opening file");
        exit(EXIT_FAILURE);
    }
    lexer->line_number = 1;
}

// Initialize the DFA with states and transitions
void init_dfa() {
    // Initialize all transitions to an error state, which we could define as START for simplicity
    for (int i = 0; i < NUM_STATES; ++i) {
        for (int j = 0; j < MAX_CHAR; ++j) {
            transition_table[i].transition[j] = START; // or another state representing an error
        }
        transition_table[i].is_final = false;
    }

    // Define transitions for digits
    for (int i = '0'; i <= '9'; ++i) {
        define_transition(START, i, INTEGER);
        define_transition(INTEGER, i, INTEGER);
    }
    define_final_state(INTEGER, TOKEN_LITERAL);
}
int place = 0;

bool read_char_equal(DFALexer *lexer, char expected) {
    char next = (char)fgetc(lexer->reader);
    if (next == expected) {
        return true;
    } else {
        ungetc(next, lexer->reader); // Put it back if it doesn't match
        return false;
    }
}

void reserve(DFALexer *lexer, TokenType type, const char *str) {
    if (lexer->reserved_count < sizeof(lexer->reserved_words) / sizeof(lexer->reserved_words[0])) {
        lexer->reserved_words[lexer->reserved_count].type = type;
        lexer->reserved_words[lexer->reserved_count].lexeme = strdup(str);
        lexer->reserved_count++;
    } else {
        fprintf(stderr, "Error: Too many reserved words\n");
        exit(EXIT_FAILURE);
    }
}

// Function to read the next character
int read_char(DFALexer *lexer) {
    return fgetc(lexer->reader);
}

Token *next_token(DFALexer *lexer,Node **symbolTable) {
    int curChar;
    bool flag = false;
    Token *token = malloc(sizeof(Token));
    if (!token) {
        fprintf(stderr, "Memory allocation failed for token.\n");
        exit(EXIT_FAILURE);
    }

    State state = START;
    token->lexeme = NULL;
    size_t lexeme_length = 0;

    while (state != DONE) {
        curChar = read_char(lexer);
        if (curChar == EOF) {
            if (state == START) {
                free(token);
                return NULL;
            } else {
                token->lexeme[lexeme_length] = '\0';
                token->len = lexer->line_number;
                state = DONE;
                break;
            }
        }

        switch (state) {
            case START:
                place++;
                if(flag && curChar != ']'){
                    continue;
                }else{
                    flag = false;
                }
                if (isspace(curChar)) {
                    if (curChar == '\n') {
                        lexer->line_number++;
                        place = 0;
                    }
                    break; // Ignore whitespaces
                } else if (isalpha(curChar)) {
                    state = IDENTIFIER;
                    token->lexeme = malloc(256);
                    token->lexeme[lexeme_length++] = curChar;
                    token->type = TOKEN_ID;
                    token->len = lexer->line_number;
                } else if (isdigit(curChar)) {
                    state = INTEGER;
                    token->lexeme = malloc(11);
                    token->lexeme[lexeme_length++] = curChar;
                    token->type = TOKEN_LITERAL;
                    token->len = lexer->line_number;
                } else {
                    switch (curChar) {
                        case '+':
                            token->type = TOKEN_PLUS;
                            state = DONE;
                            break;
                        case '-':
                            if (read_char_equal(lexer, '>')) {
                                token->type = TOKEN_ARROW;
                                token->lexeme = strdup("->");
                            } else {
                                free(token->lexeme);
                                free(token);
                                fprintf(stderr, "\"%c\"\t\t\tLexical error\n", curChar);
                                fprintf(stderr, "Error: Invalid Token '%c' found at line: %d, column %d\n", curChar,lexer->line_number,place);
                                fprintf(stderr, "Lexical Analysis Halt since an invalid token is detected!\n");
                                return 0;
                            }
                            state = DONE;
                            break;
//                        case 0xE2: { // First byte of the UTF-8 representation of en dash
//                            char next1 = (char)fgetc(lexer->reader); // Read the next byte
//                            char next2 = (char)fgetc(lexer->reader); // Read the byte after that
//                            if (next1 == (char)0x80 && next2 == (char)0x93) {
//                                curChar = '-'; // Replace with hyphen-minus
//                                // Now handle as if it was a '-' character
//                                if (read_char_equal(lexer, '>')) {
//                                    token->type = TOKEN_ARROW;
//                                    token->lexeme = strdup("->");
//                                } else {
//                                    fprintf(stderr, "\"%c\"\t\t\tLexical error\n", curChar);
//                                    fprintf(stderr, "Error: Invalid Token '%c' found at line %d %d: \n", curChar,lexer->line_number,place);
//                                    fprintf(stderr, "Lexical Analysis Halt since an invalid token is detected!\n");
//                                    return 0;
//                                }
//                            }
//                            break;
//                        }
                        case ':':
                            token->type = TOKEN_COLON;
                            state = DONE;
                            break;
                        case ';':
                            token->type = TOKEN_SEMICOLON;
                            state = DONE;
                            break;
                        case '?':
                            token->type = TOKEN_QUESTION;
                            state = DONE;
                            break;
                        case '=':
                            token->type = TOKEN_ASSIGN;
                            state = DONE;
                            break;
                        case '(':
                            token->type = Token_BrackL;
                            state = DONE;
                            break;
                        case ')':
                            token->type = Token_BrackR;
                            state = DONE;
                            break;
                        case '&':
                            token->type = Token_and;
                            state = DONE;
                            break;
                        case '<':
                            token->type = Token_less;
                            state = DONE;
                            break;
                        case'[':
                            flag = true;
                            break;
                        case']':
                            break;
                        default:
                            free(token->lexeme);
                            free(token);
                            fprintf(stderr, "\"%c\"\t\t\tLexical error\n", curChar);
                            fprintf(stderr, "Error: Invalid Token '%c' found at line: %d, column %d\n", curChar,lexer->line_number,place);
                            fprintf(stderr, "Lexical Analysis Halt since an invalid token is detected!\n");
                            return 0;
                    }
                    if (state == DONE) {
                        token->lexeme = token->lexeme ? token->lexeme : malloc(2);
                        token->len = lexer->line_number;
                        token->lexeme[0] = curChar;
                        if(curChar == '-'){
                            token->lexeme[1] = '>';
                            token->lexeme[2] = '\0';
                        }else{
                            token->lexeme[1] = '\0';
                        }
                    }
                }
                break;

            case INTEGER:
                place++;
                if (isdigit(curChar)) {
                    token->lexeme[lexeme_length++] = curChar;
                } else {
                    ungetc(curChar, lexer->reader); // Push back the non-digit
                    place--;
                    token->lexeme[lexeme_length] = '\0';
                    state = DONE;
                }
                break;

            case IDENTIFIER:
                place++;
                if (isalnum(curChar)) {
                    token->lexeme[lexeme_length++] = curChar;
                } else {
                    ungetc(curChar, lexer->reader); // Push back the non-alphanumeric
                    token->lexeme[lexeme_length] = '\0';
                    // Determine if the lexeme is a reserved word
                    for (int i = 0; i < lexer->reserved_count; ++i) {
                        if (strcmp(lexer->reserved_words[i].lexeme, token->lexeme) == 0) {
                            token->type = lexer->reserved_words[i].type;
                            state = DONE;
                            break;
                        }
                    }
                    if (state != DONE) {
                        if (strcmp(token->lexeme, "T") == 0) {
                            token->type = TOKEN_LITERAL;
                        }else if(strcmp(token->lexeme, "F")==0){
                            token->type = TOKEN_LITERAL;
                        }
                        else {
                            token->type = TOKEN_ID;
                            Symbol symbol1;
                            strncpy(symbol1.name, token->lexeme, sizeof(symbol1.name) - 1);
                            symbol1.name[sizeof(symbol1.name) - 1] = '\0';
                            strncpy(symbol1.type, "(null)", sizeof(symbol1.type) - 1);
                            symbol1.type[sizeof(symbol1.type) - 1] = '\0';
                            insertNode(symbolTable, symbol1);
                        }
                        state = DONE;
                    }
                }
                break;
        }

    }

    return token;
}

