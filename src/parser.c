#include "scanner.h"
#include "parser.h"

int EOF = 509;

;


int FIRST_PREPROCESSOR;
int FIRST_FUNCTION_DEFINITION;
int FIRST_GLOBAL_VARIABLE_DECLARATION;


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
    if(isIn(tokenType, FIRST_PREPROCESSOR)) {
        preprocessor();
        getNextToken();
        return;
    }
    if (isIn(tokenType, FIRST_FUNCTION_DEFINITION)) {
        function_definition();
        getNextToken();
        return;        
    }
    if (isIn(tokenType, FIRST_GLOBAL_VARIABLE_DECLARATION)) {
        global_variable_declaration();
        getNextToken();
        return;        
    }

    //TODO: report error
}

void preprocessor() {
    if(tokenType == '#') {
        getNextToken();
        if(tokenType == TOKEN_INCLUDE) {
            getNextToken();
            if(tokenType == TOKEN_STRING_LITERAL) {
                getNextToken();
                // TODO: do something
            } // token == TOKEN_STRING_LITERAL
        } // token == TOKEN_INCLUDE
    } // token == '#'
}

void function_definition() {
    if(isIn(tokenType, FIRST_TYPE)) {
        type();
        getNextToken();
        if(token == TOKEN_IDENTIFIER) {
            identifier();
            getNextToken();
            if(token == '(') {
                getNextToken();
                while(isIn(token, FIRST_VARIABLE_DECLARATION)) {
                    variable_declaration();
                    getNextToken();
                }
                if(tokenType == ')') {
                    getNextToken();
                    if(tokenType == '{') {
                        getNextToken();
                        function_body();
                        getNextToken();
                        if(tokenType == '}') {
                            getNextToken();
                        } // token == '}'
                    } // token == '{'
                } // token == ')'
            } // token == '('
        } // token == TOKEN_IDENTIFIER
    } // isIn(token, FIRST_TYPE)
}

void global_variable_declaration() {
    if(tokenType == TOKEN_STATIC) {
        getNextToken();
        variable_declaration();
    }
}

void variable_declaration() {
    if(tokenType == TOKEN_STATIC) {
        getNextToken();
    }
    if(isIn(tokenType, FIRST_TYPE)) {
        getNextToken();
        if(tokenType = TOKEN_IDENTIFIER) {
            getNextToken();
            if(tokenType == TOKEN_EOS) {
                getNextToken();
            } // token == TOKEN_EOS
        } // token = TOKEN_IDENTIFIER
    } // token, FIRST_TYPE
}



void factor() {
    if(tokenType == TOKEN_IDENTIFIER) {

        return;
    }
    if(isIn(token, FIRST_TYPE)) {

        return;
    }
    if(token == TOKEN_LPARENS) {
        getNextToken();
        expression();
        if(token == TOKEN_RPARENS) {
            getNextToken();
        }
        return;
    }
    
}

// ----------------------------------------------------------------------------