#include "scanner.h"

int EOF;

EOF = 509;

// Reports errors of parser
void error()
{
	error(0);
}

void error(int token)
{
    
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