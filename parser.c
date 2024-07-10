#include "parser.h"
#include "parse_tree.h"

bool check(char* str1, char* str2){
    int len1 = strlen(str1);
    int len2 = strlen(str2);

    while (len1 > 0 && len2 > 0) {
        while (len1 > 0 && (isdigit(str1[len1 - 1]) || isspace(str1[len1 - 1])))
            len1--;
        while (len2 > 0 && (isdigit(str2[len2 - 1]) || isspace(str2[len2 - 1]) || str2[len2-1]==','))
            len2--;
        if (len1 > 0 && len2 > 0 && str1[len1 - 1] != str2[len2 - 1])
            return 0;
        len1--;
        len2--;
    }
    str2[len2] = '\0';
    return len1 == 0;
}



bool existNa(int val, char* str, int col, int *PP){
    if(map[val][col] < 0 && check(grammar[map[val][col]*(-1)].productions,str)){
        *PP = (map[val][col]*(-1));
        return true;
    }
    return false;
}

int mapCharToNonTerminal(char c) {
    switch (c) {
        case 'S':
            return S;
        case 'F':
            return F;
        case 'A':
            return A;
        case 'T':
            return T;
        case 'C':
            return C;
        case 'B':
            return B;
        case 'E':
            return E;
        case 'I':
            return I;
        case 'R':
            return R;
        default:
            return -1;
    }
}
