#include "scanner.h"
#include "parser.h"

int FIRST_FUNCTION_DEFINITION;
int FIRST_GLOBAL_VARIABLE_DECLARATION;
int FIRST_TYPE;
int FIRST_VARIABLE_DECLARATION;
int FIRST_EXPRESSION;


// Reports errors of parser

void error(int token)
{
    
}

void initTokens(){
    TOKEN_NULL = 0;
    TOKEN_VOID = 1;
    TOKEN_INT = 2;
    TOKEN_CHAR = 3;
    TOKEN_DOUBLE = 4;
    TOKEN_WHILE = 5;
    TOKEN_IF = 6;
    TOKEN_ELSE = 7;
    TOKEN_RETURN = 8;
    TOKEN_STRUCT = 9;
    TOKEN_STATIC = 10;
    
    TOKEN_IDENTIFIER = 100;
    
    TOKEN_CONSTINT = 200;
    TOKEN_CONSTDOUBLE = 201;
    TOKEN_CONSTCHAR = 202;
    
    TOKEN_STRING_LITERAL = 300;
    
    TOKEN_PLUS = 400;
    TOKEN_MINUS = 401;
    TOKEN_MULT = 402;
    TOKEN_DIVIDE = 403;
    TOKEN_PERCENT = 404;
    TOKEN_ASSIGNMENT = 405;
    TOKEN_EQUAL = 406;
    TOKEN_LESSEQUAL = 407;
    TOKEN_GREATEREQUAL = 408;
    TOKEN_LESS = 409;
    TOKEN_GREATER = 410;
    TOKEN_SHIFTLEFT = 411;
    TOKEN_SHIFTRIGHT = 412;
    TOKEN_ADDRESS = 413;
    TOKEN_AND = 414;
    TOKEN_BITWISEOR = 415;
    TOKEN_OR = 416;
    TOKEN_BITWISEEXCLOR = 417;
    TOKEN_BITWISENOT = 418;
    TOKEN_UNEQUAL = 419;
    TOKEN_INCREMENT = 420;
    TOKEN_DECREMENT = 421;
    TOKEN_NOT = 422;
    
    TOKEN_LSB = 500;
    TOKEN_RSB = 501;
    TOKEN_LRB = 502;
    TOKEN_RRB = 503;
    TOKEN_LCB = 504;
    TOKEN_RCB = 505;
    TOKEN_SEMICOLON = 506;
    TOKEN_COMMA = 507;
    TOKEN_COLON = 508;
    TOKEN_EOF = 509;
    TOKEN_INCLUDE = 510;
    TOKEN_POINT = 511;
    
    FIRST_EXPRESSION = 0;
    FIRST_FUNCTION_DEFINITION = 1;
    FIRST_GLOBAL_VARIABLE_DECLARATION = 2;
    FIRST_TYPE = 3;
    FIRST_VARIABLE_DECLARATION = 4;
}


int isIn(int tokenType, int rule){
    
    if((tokenType == 1 || tokenType == 2 || tokenType == 3) && rule == 1){return 1;} // function_definition
    if(tokenType == 10 && rule == 2){return 1;} //variable_declaration global

    return 0;
    
}

// -------------------------- EBNF --------------------------------------------

void start() {

    while(tokenType == TOKEN_INCLUDE)
    {
        getNextToken();
        if(tokenType == TOKEN_STRING_LITERAL)
        {
            getNextToken();
            //TODO: handle includes
        }
    }

    while(tokenType != TOKEN_EOF) {
        top_declaration();
        getNextToken();
    }
}

void top_declaration() {
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

void function_definition() {
    if(isIn(tokenType, FIRST_TYPE)) {
        type();
        getNextToken();
        if(tokenType == TOKEN_IDENTIFIER) {
            getNextToken();
            if(tokenType == '(') {
                getNextToken();
                while(isIn(tokenType, FIRST_VARIABLE_DECLARATION)) {
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
        if(tokenType == TOKEN_IDENTIFIER) {
            getNextToken();
            if(tokenType == TOKEN_EOF) {
                getNextToken();
            } // token == TOKEN_EOS
        } // token = TOKEN_IDENTIFIER
    } // token, FIRST_TYPE
}

void function_body()
{
    if(tokenType == TOKEN_LCB) {
        getNextToken();
        while(tokenType != TOKEN_RCB)
        {
            function_statement();
        }
    } // tokenType == TOKEN_LCB
}

void function_statement()
{
    if(isIn(tokenType, FIRST_VARIABLE_DECLARATION))
    {
        variable_declaration();
    }
    else if(tokenType == TOKEN_IF)
    {
        if_else();
    }
    else if(tokenType == TOKEN_WHILE)
    {
        while_loop();
    }
    else if(isIn(tokenType, FIRST_EXPRESSION))
    {
        expression();
    }
}

void if_else()
{
    if(tokenType == TOKEN_IF)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            if(isIn(tokenType, FIRST_EXPRESSION))
            {
                getNextToken();
                if(tokenType == TOKEN_RRB)
                {
                    getNextToken();
                    function_body();
                    if(tokenType == TOKEN_RCB)
                    {
                        getNextToken();
                    }
                    //TODO: else branch
                }
            }
        }
    }
}

void while_loop()
{
    if(tokenType == TOKEN_WHILE)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            expression();
            if(tokenType == TOKEN_RRB)
            {
                getNextToken();
                if(tokenType == TOKEN_LCB)
                {
                    function_body();
                    if(tokenType == TOKEN_RCB)
                    {
                        getNextToken();
                    }
                }
            }
        }   

    }
}

void expression()
{
    simple_expression();
    if(tokenType == TOKEN_EQUAL)
    {
        getNextToken();
        simple_expression();
    }
    if(tokenType == TOKEN_LESSEQUAL)
    {
        getNextToken();
        simple_expression();
    }
    if(tokenType == TOKEN_LESS)
    {
        getNextToken();
        simple_expression();
    }
    if(tokenType == TOKEN_UNEQUAL)
    {
        getNextToken();
        simple_expression();
    }
    if(tokenType == TOKEN_GREATER)
    {
        getNextToken();
        simple_expression();
    }
    if(tokenType == TOKEN_GREATEREQUAL)
    {
        getNextToken();
        simple_expression();
    }
    if(tokenType == TOKEN_AND)
    {
        getNextToken();
        simple_expression();
    }
    if(tokenType == TOKEN_OR)
    {
        getNextToken();
        simple_expression();
    }
}

void simple_expression()
{
    if(tokenType == TOKEN_MINUS)
    {
        getNextToken();
        //TODO: handle optional minus
    }

    term();
    while(tokenType == TOKEN_PLUS || tokenType == TOKEN_MINUS)
    {
        getNextToken();
        term();
    }
}

void term()
{
    factor();
    while(tokenType == TOKEN_MULT || tokenType == TOKEN_DIVIDE)
    {
        getNextToken();
        factor();
    }
}

void type()
{
    if(tokenType == TOKEN_INT)
    {
        getNextToken();
        return;
    }
    if(tokenType == TOKEN_CHAR)
    {
        getNextToken();
        return;
    }
}

void factor() {
    if(tokenType == TOKEN_IDENTIFIER) {

        return;
    }
    if(isIn(tokenType, FIRST_TYPE)) {

        return;
    }
    if(tokenType == TOKEN_LSB) {
        getNextToken();
        expression();
        if(tokenType == TOKEN_RSB) {
            getNextToken();
        }
        return;
    }
    
}

// ----------------------------------------------------------------------------