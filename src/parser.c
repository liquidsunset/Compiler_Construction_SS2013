#include "scanner.c"

void function_body();
void expression();

void error(int token)
{
    printf("Expected token %d near Line %d, Col %d", token, positionLine, positionColumn);
}

int isIn(int tokenType, int rule){
    if(rule == FIRST_EXPRESSION && (tokenType == TOKEN_MINUS || tokenType == TOKEN_IDENTIFIER || tokenType == TOKEN_CONSTINT || tokenType == TOKEN_CONSTCHAR || tokenType == TOKEN_LRB || tokenType ==TOKEN_STRING_LITERAL)){return 1;}
    if(rule == FIRST_FUNCTION_DEFINITION && (tokenType == TOKEN_VOID || tokenType == TOKEN_INT || tokenType == TOKEN_CHAR)){return 1;} // function_definition
    if(rule == FIRST_GLOBAL_VARIABLE_DECLARATION && tokenType == TOKEN_STATIC){return 1;} //variable_declaration global
    if(rule == FIRST_TYPE && (tokenType == TOKEN_INT || tokenType == TOKEN_CHAR || tokenType == TOKEN_VOID)){return 1;}
    if(rule == FIRST_VARIABLE_DECLARATION && (tokenType == TOKEN_STATIC || isIn(tokenType, FIRST_TYPE))) { return 1; }
    if(rule == FIRST_FUNCTION_STATEMENT && (isIn(tokenType, FIRST_VARIABLE_DECLARATION) || tokenType == TOKEN_WHILE || tokenType == TOKEN_IF || isIn(tokenType, FIRST_EXPRESSION))) { return 1; }

    return 0;
}

// -------------------------- EBNF --------------------------------------------

void factor() {
    if(tokenType == TOKEN_IDENTIFIER) {
        getNextToken();
        if(tokenType == TOKEN_LRB) // function call
        {
            getNextToken();
            while(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression();
                if(tokenType == TOKEN_COMMA)
                {
                    getNextToken();
                }
            }
            if(tokenType == TOKEN_RRB)
            {
                getNextToken();
            }
            return;
        }
        if(tokenType == TOKEN_LSB)
        {
            getNextToken();
            if(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression();
            }
            if(tokenType == TOKEN_RSB)
            {
                getNextToken();
            }
            return;
        }
    }
    if(tokenType == TOKEN_LSB) {
        getNextToken();
        expression();
        if(tokenType == TOKEN_RSB) {
            getNextToken();
        }
        return;
    }
    if(tokenType == TOKEN_CONSTINT)
    {
        getNextToken();
        return;
    }
    if(tokenType == TOKEN_CONSTCHAR)
    {
        getNextToken();
        return;
    }
    if(tokenType == TOKEN_STRING_LITERAL){
        getNextToken();
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
    if(tokenType == TOKEN_VOID)
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
    if(tokenType == TOKEN_ASSIGNMENT)
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
                expression();
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
        type();
        if(tokenType == TOKEN_IDENTIFIER) {
            getNextToken();
        } // token = TOKEN_IDENTIFIER
    } // token, FIRST_TYPE
}

void function_statement()
{
    if(isIn(tokenType, FIRST_VARIABLE_DECLARATION))
    {
        variable_declaration();
        if(tokenType == TOKEN_SEMICOLON)
        {
            getNextToken();
        }
    }
    else if(tokenType == TOKEN_IF)
    {
        if_else();
    }
    else if(tokenType == TOKEN_WHILE)
    {
        while_loop();
    }
    else if(tokenType == TOKEN_RETURN)
    {
        getNextToken();
        if(isIn(tokenType, FIRST_EXPRESSION))
        {
            expression();
        }
    }
    else if(isIn(tokenType, FIRST_EXPRESSION))
    {
        expression();
        if(tokenType == TOKEN_SEMICOLON)
        {
            getNextToken();
        }
    }
}

void function_body()
{
    if(tokenType == TOKEN_LCB) {
        getNextToken();
        while(isIn(tokenType, FIRST_FUNCTION_STATEMENT))
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
        if(tokenType == TOKEN_IDENTIFIER) {
            getNextToken();
            if(tokenType == TOKEN_LRB) {
                getNextToken();
                while(isIn(tokenType, FIRST_VARIABLE_DECLARATION)) {
                    variable_declaration();
                    if(tokenType == TOKEN_COMMA)
                    {
                        getNextToken();
                    }
                }
                if(tokenType == TOKEN_RRB) {
                    //getNextToken();
                    //if(tokenType == '{') {
                        getNextToken();
                        function_body();
                        getNextToken();
                        if(tokenType == TOKEN_RCB) {
                            getNextToken();
                        } // token == '}'
                   // } // token == '{'
                } // token == ')'
            } // token == '('
        } // token == TOKEN_IDENTIFIER
    } // isIn(token, FIRST_TYPE)
}

void top_declaration() {
    if (isIn(tokenType, FIRST_FUNCTION_DEFINITION)) {
        function_definition();
        //getNextToken();
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
        //getNextToken();
    }
}

int main(){
    printf("Phoenix: Parser\n");
    initTokens();
    openFile("src/parser.c");
    //openFile("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/src/parser.c");
    start();
    printf("The End\n");

    return 0;
}

// ----------------------------------------------------------------------------