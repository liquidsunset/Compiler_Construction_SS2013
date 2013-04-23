#include "scanner.c"

void function_body();
void expression();

void error(char message[1024])
{
    errorCount = errorCount + 1;
    printf("Error Near Line %d, Col %d: %s", positionLine, positionColumn, message);
}

void mark(char message[1024])
{
    printf("Warning Near Line %d, Col %d: %s", positionLine, positionColumn, message);
}

int isIn(int tokenType, int rule){
    if(rule == FIRST_EXPRESSION && (tokenType == TOKEN_MINUS || tokenType == TOKEN_IDENTIFIER || tokenType == TOKEN_CONSTINT || tokenType == TOKEN_CONSTCHAR || tokenType ==TOKEN_STRING_LITERAL)){return 1;}
    if(rule == FIRST_FUNCTION_DEFINITION && (tokenType == TOKEN_VOID || tokenType == TOKEN_INT || tokenType == TOKEN_CHAR)){return 1;} // function_definition
    if(rule == FIRST_GLOBAL_VARIABLE_DECLARATION && tokenType == TOKEN_STATIC){return 1;} //variable_declaration global
    if(rule == FIRST_TYPE && (tokenType == TOKEN_INT || tokenType == TOKEN_CHAR || tokenType == TOKEN_VOID)){return 1;}
    if(rule == FIRST_VARIABLE_DECLARATION && (tokenType == TOKEN_STATIC || isIn(tokenType, FIRST_TYPE))) { return 1; }
    if(rule == FIRST_FUNCTION_STATEMENT && (isIn(tokenType, FIRST_VARIABLE_DECLARATION) || tokenType == TOKEN_WHILE || tokenType == TOKEN_IF || tokenType == TOKEN_RETURN || isIn(tokenType, FIRST_EXPRESSION))) { return 1; }

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
            else
            {
                mark(") missing (factor)");
                getNextToken();
            }
        }
        if(tokenType == TOKEN_LSB) // array
        {
            getNextToken();
            if(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression();
            }
            else
            {
                error("Expression for index expected (factor)");
            }
            if(tokenType == TOKEN_RSB)
            {
                getNextToken();
            }
            else
            {
                mark("] missing (factor)");
                getNextToken();
            }
        }
        return;
    }
    // if(tokenType == TOKEN_LSB) {
    //     getNextToken();
    //     expression();
    //     if(tokenType == TOKEN_RSB) {
    //         getNextToken();
    //     }
    //     else
    //     {
    //         mark(") missing");
    //         getNextToken();
    //     }
    //     return;
    // }
    if(tokenType == TOKEN_LRB)
    {
        getNextToken();
        expression();
        if(tokenType == TOKEN_RRB)
        {
            getNextToken();
            return;
        }
        else
        {
            mark(") missing (factor)");
            getNextToken();
            return;
        }
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
    
    error("Factor expected (factor)");
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

    error("Type expected (type)");
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
        expression();
        return;
    }
    if(tokenType == TOKEN_LESSEQUAL)
    {
        getNextToken();
        expression();
        return;
    }
    if(tokenType == TOKEN_LESS)
    {
        getNextToken();
        expression();
        return;
    }
    if(tokenType == TOKEN_UNEQUAL)
    {
        getNextToken();
        expression();
        return;
    }
    if(tokenType == TOKEN_GREATER)
    {
        getNextToken();
        expression();
        return;
    }
    if(tokenType == TOKEN_GREATEREQUAL)
    {
        getNextToken();
        expression();
        return;
    }
    if(tokenType == TOKEN_AND)
    {
        getNextToken();
        expression();
        return;
    }
    if(tokenType == TOKEN_OR)
    {
        getNextToken();
        expression();
        return;
    }
    if(tokenType == TOKEN_ASSIGNMENT)
    {
        getNextToken();
        expression();
        return;
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
                    //TODO: else branch
                    if(tokenType == TOKEN_ELSE)
                    {
                        getNextToken();
                        function_body();
                    }

                }
                else
                {
                    error(") after if is missing (if_else)");
                }
            }
        }
        else
        {
            error("( after if is missing (if_else)");
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
                function_body();
            }
            else
            {
                    error(") after while is missing (while_loop)");
            }
        }
        else
        {
            error("( after while is missing (while_loop)");
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
            if(tokenType == TOKEN_LSB) // array
            {
                getNextToken();
                if(isIn(tokenType, FIRST_EXPRESSION))
                {
                    expression();
                }
                else
                {
                    error("Expression for index expected (variable_declaration)");
                }
                if(tokenType == TOKEN_RSB)
                {
                    getNextToken();
                }
                else
                {
                    mark("] missing (variable_declaration)");
                    getNextToken();
                }
            }
        } // token = TOKEN_IDENTIFIER
        else
        {
            error("Identifier missing (variable_declaration)");
        }
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
        else
        {
            mark("; missing (function_statement)");
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
        if(tokenType == TOKEN_SEMICOLON)
        {
            getNextToken();
        }
        else
        {
            mark("; missing (function_statement)");
            getNextToken();
        }
    }
    else if(isIn(tokenType, FIRST_EXPRESSION))
    {
        expression();
        if(tokenType == TOKEN_SEMICOLON)
        {
            getNextToken();
        }
        else
        {
            mark("; missing (function_statement)");
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
        if(tokenType == TOKEN_RCB) {
            getNextToken();
        }
        else
        {
            mark("} missing (function_body)");
            getNextToken();
        }
    } // tokenType == TOKEN_LCB
}

void global_variable_declaration() {
    if(tokenType == TOKEN_STATIC) {
        getNextToken();
        variable_declaration();
    }
    else
    {
        error("global variables must be static (global_variable_declaration)");
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
                    getNextToken();
                    if(tokenType == TOKEN_LCB)
                    {
                        function_body();
                        return;
                    }
                    if(tokenType == TOKEN_SEMICOLON)
                    {
                        getNextToken();
                        return;
                    }
                    error("expected { or ; (function_definition)");
                     // token == '}'
                } // token == ')'
                else
                {
                    error(") missing (function_definition)");
                }
            } // token == '('
            else
            {
                error("( after function identifier expected (function_definition)");
            }
        } // token == TOKEN_IDENTIFIER
        else
        {
            error("Identifier expected (function_definition)");
        }
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

    error("Variable or function declaration expected (top_declaration)");
}

void start() {
    getNextToken();
    while(tokenType == TOKEN_INCLUDE)
    {
        getNextToken();
        if(tokenType == TOKEN_STRING_LITERAL)
        {
            getNextToken();
        }
        else
        {
            error("Expected string literal (start)");
        }
    }

    while(tokenType != TOKEN_EOF) {
        if(isIn(tokenType, FIRST_TYPE))
        {
            top_declaration();
            getchar();    
        }
        else
        {
            getNextToken();
        }
    }
}

int main(){
    printf("Phoenix: Parser\n");
    initTokens();
    openFile("test/easy.c");
    //openFile("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/test/easy.c");
    start();
    printf("Parsed with %d\n", errorCount);

    return 0;
}

// ----------------------------------------------------------------------------