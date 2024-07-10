#include "lexer.h"
#include "parser.h"
#include "parse_tree.h"

int PP = 0;

#define MAX_TOKENS 1000

int main(int argc, char *argv[]) {
    Node *symbolTable = NULL;
    DFALexer lexer;
    lexer_init(&lexer);
    char filename[50];
    scanf("%s", filename);
//    char *filename = argv[1];
    lexer_load_code(&lexer, filename);
    Token **tokens = malloc(MAX_TOKENS * sizeof(Token*));
    Token *token;
    size_t tokenCount = 0;
    while ((token = next_token(&lexer, &symbolTable)) != NULL && tokenCount < MAX_TOKENS) {
        if(token->type == TOKEN_ID){
            if (strcmp(token->lexeme, "T") == 0) {
                token->type = TOKEN_LITERAL;
            }else if(strcmp(token->lexeme, "F")==0){
                token->type = TOKEN_LITERAL;
            }else if (strcmp(token->lexeme, "fun") == 0) {
                token->type = TOKEN_FUN;
            }else if(strcmp(token->lexeme, "nat")==0){
                token->type = TOKEN_NAT;
            }else if(strcmp(token->lexeme, "bool")==0){
                token->type = TOKEN_BOOL;
            }
        }
        tokens[tokenCount++] = token;
    }
    int time = 0;
    int L = strlen(filename);
    char NFiname[50];
    for(int i = 0;i < L;i++){
        if(filename[i] != '.'){
            NFiname[i] = filename[i];
        }else{
            NFiname[i] = '\0';
            break;
        }
    }
    strcat(NFiname, "_out.txt");
    printf("%s\n",NFiname);
    FILE *file = fopen(NFiname,"w");
    if (file == NULL) {
        perror("Error opening file");
        return 1;
    }
    freopen(NFiname, "w", stdout);
    char str[] = ":(0,";
    printf("Step %d%s\n",time++,str);
    for (int i = 0; i < tokenCount; ++i) {
        int len = tokens[i]->type;
        int length = strlen(str);
        int index = length - 1;
        while (index >= 0 && !isdigit(str[index])) {
            index--;
        }
        char number[10]; // 假设数字不超过10位
        int j = 0;
        while (index >= 0 && isdigit(str[index])) {
            number[j++] = str[index--];
        }
        number[j] = '\0';
        for (int k = 0; k < j / 2; k++) {
            char temp = number[k];
            number[k] = number[j - k - 1];
            number[j - k - 1] = temp;
        }
        int extractedNumber = atoi(number);
        int place = map[extractedNumber][len];
        char tempStr[20];  // Adjust the size accordingly
        if (len == 2) {
            sprintf(tempStr, " id %d,", place);
        } else if (len == 10) {
            sprintf(tempStr, " lit %d,", place);
        } else {
            sprintf(tempStr, " %s %d,", tokens[i]->lexeme, place);
        }
        strcat(str, tempStr);
        if(place == 0){
            printf("Step %d%s %-70s syntax error\n", time++, str, "");
            fclose(stdout);
            freopen("CON", "w", stdout);
            printf("Syntax error on Line %d!",tokens[i]->len);
            exit(0);
        }
        printf("Step %d%-80s shift %d\n", time++, str, place);
        if(i < tokenCount - 1 && map[place][tokens[i+1]->type] < 0){
            while(existNa(place,str,tokens[i+1]->type, &PP)){
                length = strlen(str);
                index = length - 1;
                while (index >= 0 && !isdigit(str[index])) {
                    index--;
                }
                char number1[10];
                j = 0;
                while (index >= 0 && isdigit(str[index])) {
                    number[j++] = str[index--];
                }
                number[j] = '\0';
                for (int k = 0; k < j / 2; k++) {
                    char temp = number[k];
                    number[k] = number[j - k - 1];
                    number[j - k - 1] = temp;
                }
                extractedNumber = atoi(number);
                int a = mapCharToNonTerminal(grammar[PP].nonTerminal);
                char M[2];
                M[0] = grammar[PP].nonTerminal, M[1] = '\0';
                char tempStr[20];
                place = map[extractedNumber][a];
                sprintf(tempStr, "%s %d,", M, place);
                strcat(str, tempStr);
                printf("Step %d%-80s reduce %c -> %s\n", time++, str,grammar[PP].nonTerminal, grammar[PP].productions);
            }
        }else if(i == tokenCount - 1){
            while(existNa(place,str,15,&PP)){
                length = strlen(str);
                index = length - 1;
                while (index >= 0 && !isdigit(str[index])) {
                    index--;
                }
                char number1[10]; // 假设数字不超过10位
                j = 0;
                while (index >= 0 && isdigit(str[index])) {
                    number[j++] = str[index--];
                }
                number[j] = '\0';
                for (int k = 0; k < j / 2; k++) {
                    char temp = number[k];
                    number[k] = number[j - k - 1];
                    number[j - k - 1] = temp;
                }
                extractedNumber = atoi(number);
                int a = mapCharToNonTerminal(grammar[PP].nonTerminal);
                char M[2];
                M[0] = grammar[PP].nonTerminal, M[1] = '\0';
                char tempStr[20];
                place = map[extractedNumber][a];
                sprintf(tempStr, "%s %d,", M, place);
                strcat(str, tempStr);
                printf("Step %d%-80s reduce %c -> %s\n", time++, str,grammar[PP].nonTerminal, grammar[PP].productions);
            }
        }
        if(place == 1 && i == tokenCount - 1){
            printf("Step %d%s $%-70s accept\n", time++, str, "");
        }else if(i == tokenCount - 1){
            printf("Step %d%s $%-70s syntax error\n", time++, str, "");
            fclose(stdout);
            freopen("CON", "w", stdout);
            printf("Syntax error on Line %d!",tokens[i]->len);
            exit(0);
        }
    }
    fclose(stdout);
    freopen("CON", "w", stdout);
    printf("Parsing Accomplished! No syntax error!");
    return 0;
}
