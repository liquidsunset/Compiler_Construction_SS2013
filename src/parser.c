#include "scanner.c"

static int currentType;

// ------------------------------- Symbol table -------------------------------

struct object_t;

struct type_t {
    int form;
    struct object_t *fields;
    struct type_t *base;
};

struct object_t{
    char *name;
    int class;
    struct type_t *type_t;
    struct object_t *next;
};

void getFromList()
{
    printf("Reading %s\n", stringValue);
}

struct object_t *global;
struct object_t *local;

void addToList()
{
    printf("Adding %s\n", stringValue);
}

void addToListWIP(){
    struct object_t *newElement;
    newElement = malloc(sizeof(struct object_t));
    newElement->name = stringValue;
    
    newElement->next = 0;
    printf("%s\n",stringValue);
}
// -----------------------------------------------------------------------------

// --------------------Parser error reporting ----------------------------------

void error(char message[1024])
{
    int niceLine;
    int niceColumn;

    niceLine = positionLine;
    niceColumn = positionColumn;
    if(niceColumn < 0)
    {
        niceLine = niceLine-1;
    }

    errorCount = errorCount + 1;

    printf("Error Near Line %d: %s\n", niceLine, message);
}

void mark(char message[1024])
{
    int niceLine;
    int niceColumn;

    niceLine = positionLine;
    niceColumn = positionColumn;
    if(niceColumn < 0)
    {
        niceLine = niceLine-1;
    }
    
    warningCount = warningCount + 1;

    printf("Warning Near Line %d: %s\n", niceLine, message);
}
// -----------------------------------------------------------------------------

int isIn(int tokenType, int rule){
    if(rule == FIRST_EXPRESSION && (tokenType == TOKEN_MINUS || tokenType == TOKEN_IDENTIFIER || tokenType == TOKEN_CONSTINT || tokenType == TOKEN_CONSTCHAR || tokenType ==TOKEN_STRING_LITERAL || tokenType == TOKEN_LRB || tokenType == TOKEN_FCLOSE || tokenType ==
        TOKEN_FOPEN || tokenType == TOKEN_SIZEOF || tokenType == TOKEN_MALLOC)){return 1;}
    if(rule == FIRST_FUNCTION_DEFINITION && (tokenType == TOKEN_VOID || tokenType == TOKEN_INT || tokenType == TOKEN_CHAR)){return 1;} // function_definition
    if(rule == FIRST_GLOBAL_VARIABLE_DECLARATION && tokenType == TOKEN_STATIC){return 1;} //variable_declaration global
    if(rule == FIRST_TYPE && (tokenType == TOKEN_INT || tokenType == TOKEN_CHAR || tokenType == TOKEN_VOID || tokenType == TOKEN_STRUCT)){return 1;}
    if(rule == FIRST_VARIABLE_DECLARATION && (tokenType == TOKEN_STATIC || isIn(tokenType, FIRST_TYPE))) { return 1; }
    if(rule == FIRST_FUNCTION_STATEMENT && (isIn(tokenType, FIRST_VARIABLE_DECLARATION) || tokenType == TOKEN_WHILE || tokenType == TOKEN_IF || tokenType == TOKEN_RETURN || isIn(tokenType, FIRST_EXPRESSION))) { return 1; }

    return 0;
}

// -------------------------- EBNF --------------------------------------------

void function_body();
void expression();

void type()
{
    if(tokenType == TOKEN_INT)
    {
        currentType = TOKEN_INT;
        getNextToken();
        return;
    }

    if(tokenType == TOKEN_CHAR)
    {
        currentType = TOKEN_CHAR;
        getNextToken();
        return;
    }

    if(tokenType == TOKEN_VOID)
    {
        currentType = TOKEN_VOID;
        getNextToken();
        return;
    }
    
    if(tokenType == TOKEN_STRUCT)
    {
        getNextToken();
        if(tokenType == TOKEN_IDENTIFIER)
        {
            tokenType = TOKEN_STRUCT;
            getFromList();
            getNextToken();
            return;
        }
        else
        {
            error("identifier expected (type)");
        }
    }
    
    error("Type expected (type)");
}

void sizeof_func()
{
    if(tokenType == TOKEN_SIZEOF)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            
            if(isIn(tokenType, FIRST_TYPE))
            {
                type();
                if(tokenType == TOKEN_RRB)
                {
                    getNextToken();
                }
            }
            else
            {
                error("type expected (sizeof)");
            }
        }
        else
        {
            mark("( expected (sizeof)");
            getNextToken();
        }
    }
}

void malloc_func()
{
    if(tokenType == TOKEN_MALLOC)
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
                }
            }
            else
            {
                error("expression expected (malloc)");
            }
        }
        else
        {
            mark("( expected (malloc)");
            getNextToken();
        }
    }
}

void fopen_func()
{
    if(tokenType == TOKEN_FOPEN)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            if(tokenType == TOKEN_IDENTIFIER)
            {
                getFromList();
                getNextToken();
                if(tokenType == TOKEN_COMMA)
                {
                    getNextToken();
                    if(tokenType == TOKEN_STRING_LITERAL)
                    {
                        getNextToken();
                        if(tokenType == TOKEN_RRB)
                        {
                            getNextToken();
                        }
                        else
                        {
                            mark(") expected (factor)");
                            getNextToken();
                        }
                    }
                    else
                    {
                        mark("File mode expected (factor)");
                        getNextToken();
                    }
                }
                else
                {
                    mark("Comma expected");
                    getNextToken();
                }
            }
            else
            {
                mark("Identifier expected (factor)");
                getNextToken();
            }
        }
        else
        {
            mark("( expected (factor)");
            getNextToken();
        }
    }
}

void fclose_func()
{
    if(tokenType == TOKEN_FCLOSE)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            if(tokenType == TOKEN_IDENTIFIER)
            {
                getFromList();
                getNextToken();
                if(tokenType == TOKEN_RRB)
                {
                    getNextToken();
                }
                else
                {
                    mark(") expected (factor)");
                    getNextToken();
                }
            }
            else
            {
                mark("Identifier expected (factor)");
                getNextToken();
            }
        }
        else
        {
            mark("( expected (factor)");
            getNextToken();
        }
    }
}

void fgetc_func()
{
    if(tokenType == TOKEN_FGETC)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            if(tokenType == TOKEN_IDENTIFIER)
            {
                getFromList();
                getNextToken();
                if(tokenType == TOKEN_RRB)
                {
                    getNextToken();
                    return;
                }
                else
                {
                    mark(") expected (factor)");
                    getNextToken();
                }
            }
            else
            {
                mark("Identifier expected (factor)");
                getNextToken();
            }
        }
        else
        {
            mark("( expected (factor)");
            getNextToken();
        }
    }
}

void fputc_func()
{
    if(tokenType == TOKEN_FPUTC)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            if(tokenType == TOKEN_IDENTIFIER)
            {
                getFromList();
                getNextToken();
                if(tokenType == TOKEN_RRB)
                {
                    getNextToken();
                }
                else
                {
                    mark(") expected (factor)");
                    getNextToken();
                }
            }
            else
            {
                mark("Identifier expected (factor)");
                getNextToken();
            }
        }
        else
        {
            mark("( expected (factor)");
            getNextToken();
        }
    }
}

void factor() {
    if(tokenType == TOKEN_MULT) // reference
    {
        getNextToken();
    }
    
    if(tokenType == TOKEN_IDENTIFIER) {
        getFromList();
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
        if(tokenType == TOKEN_ACCESS) // record
        {
            getNextToken();
            if(tokenType == TOKEN_IDENTIFIER)
            {
                getFromList();
                getNextToken();
                if(tokenType == TOKEN_LSB)
                {
                    getNextToken();
                    expression();
                    if(tokenType == TOKEN_RSB)
                    {
                        getNextToken();
                    }
                    else
                    {
                        error("] expected (record access)");
                    }
                }
            }
            else
            {
                error("identifier expected (record access)");
            }
        }
        return;
    }

    if(tokenType == TOKEN_SIZEOF)
    {
        sizeof_func();
        return;
    }

    if(tokenType == TOKEN_MALLOC)
    {
        malloc_func();
        return;
    }

    if(tokenType == TOKEN_FOPEN)
    {
        fopen_func();
        return;
    }

    if(tokenType == TOKEN_FCLOSE)
    {
        fclose_func();
        return;
    }

    if(tokenType == TOKEN_FGETC)
    {
        fgetc_func();
        return;
    }

    if(tokenType == TOKEN_FPUTC)
    {
        fputc_func();
        return;
    }

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
    if(tokenType == TOKEN_NOT) {
        getNextToken();
    }

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
    if(tokenType == TOKEN_STRUCT)
    {
        getNextToken();
        if(tokenType == TOKEN_IDENTIFIER)
        {
            getFromList(); // struct has to be declared already
            getNextToken();

            if(tokenType == TOKEN_MULT) // reference
            {
                getNextToken();
            }

            if(tokenType == TOKEN_IDENTIFIER)
            {
                addToList();
                getNextToken();
                return;
            }
        }
        else
        {
            error("identifier expected (variable_declaration)");
        }
    }

    if(isIn(tokenType, FIRST_TYPE)) {
        type();
        if(tokenType == TOKEN_MULT) // reference
        {
            getNextToken();
        }

        if(tokenType == TOKEN_IDENTIFIER) {
            if(addToList() < 0)
            {
                error("Double declaration of variable");
                return;
            }
            getNextToken();
            // if(tokenType == TOKEN_LSB) // array
            // {
            //     getNextToken();
            //     if(isIn(tokenType, FIRST_EXPRESSION))
            //     {
            //         expression();
            //     }
            //     else
            //     {
            //         error("Expression for index expected (variable_declaration)");
            //     }
            //     if(tokenType == TOKEN_RSB)
            //     {
            //         getNextToken();
            //     }
            //     else
            //     {
            //         mark("] missing (variable_declaration)");
            //         getNextToken();
            //     }
            // }
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
        if(tokenType == TOKEN_IDENTIFIER)
        {
            addToList();
            getNextToken();
            if(tokenType == TOKEN_LRB)
            {
                getNextToken();
                while(isIn(tokenType, FIRST_VARIABLE_DECLARATION))
                {
                    variable_declaration();
                    if(tokenType == TOKEN_COMMA)
                    {
                        getNextToken();
                    }
                }
                if(tokenType == TOKEN_RRB)
                {
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

void struct_def() {
    if(tokenType == TOKEN_STRUCT)
    {
        getNextToken();
        if(tokenType == TOKEN_IDENTIFIER)
        {
            addToList();
            getNextToken();
            if(tokenType == TOKEN_LCB)
            {
                getNextToken();
                while(isIn(tokenType, FIRST_VARIABLE_DECLARATION))
                {
                    variable_declaration();
                    if(tokenType == TOKEN_SEMICOLON)
                    {
                        getNextToken();
                    }
                    else
                    {
                        mark("; expected (struct)");
                        getNextToken();
                    }
                }
                if(tokenType == TOKEN_RCB)
                {
                    getNextToken();
                    if(tokenType == TOKEN_SEMICOLON)
                    {
                        getNextToken();
                    }
                    else {
                        mark("; expected (struct)");
                        getNextToken();
                    }
                }
                else
                {
                    mark("} expected (struct)");
                    getNextToken();
                }
            }
            else
            {
                error("{ expected (struct)");
            }
        }
        else
        {
            error("identifier expected (struct)");
        }
    }
}

void top_declaration() {
    if(tokenType == TOKEN_STRUCT) {
        struct_def();
        return;
    }

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
        if(tokenType == TOKEN_STRUCT || isIn(tokenType, FIRST_TYPE) || isIn(tokenType, FIRST_GLOBAL_VARIABLE_DECLARATION))
        {
            top_declaration();
            //getchar();    
        }

        else
        {
            getNextToken();
        }
        
    }
}
// ----------------------------------------------------------------------------

int main(){
    printf("Phoenix: Parser\n");
        
    initTokens();

    errorCount = 0;
    warningCount = 0;
    tokenType = -1;
    openFile("test/m4.c");
    //openFile("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/test/m4.c");
    start();
    printf("Parsed with %d errors, %d warnings\n", errorCount, warningCount);


    return 0;
}
