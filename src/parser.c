#include "scanner.h"

int EOF;

EOF = 509;

// Reports errors of parser

void error(int token)
{
    
}


int isIn(int tokenType, int rule){
    
    if(tokenType == 510 && rule == 0){ return 1;} // preprocessor
    if((tokenType == 1 || tokenType == 2 || tokenType == 3) && rule == 1){return 1;} // function_definition
    if(tokenType == 10 && rule == 2){return 1;} //variable_declaration global
    
    return 0;
    
}

// -------------------------- EBNF --------------------------------------------

void start() {
    while(tokenType != EOF) {
        top_declaration();
        getNextToken();
    }
}

void top_declaration() {
    if(isIn(token, preprocessor)) {
        preprocessor();
        getNextToken();
    }
    else {}
}

// ----------------------------------------------------------------------------