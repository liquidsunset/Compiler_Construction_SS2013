#include "scanner.c"

// Reports errors of parser

void error(int token)
{
    
}



int isIn(int tokenType, int rule){
    
    if((tokenType == 1 || tokenType == 2 || tokenType == 3) && rule == 1){return 1;} // function_definition
    if(tokenType == 10 && rule == 2){return 1;} //variable_declaration global

    return 0;
    
}

// -------------------------- EBNF --------------------------------------------

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

void term()
{
    factor();
    while(tokenType == TOKEN_MULT || tokenType == TOKEN_DIVIDE)
    {
        getNextToken();
        factor();
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

void global_variable_declaration() {
    if(tokenType == TOKEN_STATIC) {
        getNextToken();
        variable_declaration();
    }
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

void start() {
    getNextToken();
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

int yolo(){
    initTokens();
    openFile("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/compilerbau/compilerbau/test.txt");
    start();
}

// ----------------------------------------------------------------------------