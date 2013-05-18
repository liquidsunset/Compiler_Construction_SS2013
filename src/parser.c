#include <stdlib.h>
#include "scanner.c"

static int currentType;    //0 = int, 1 = char
static char typeName[1024]; //name from struct or array
static int isArray;
static int isStruct;
static int isGlobal; // 0 for local, 1 for global
static int objectClass;

static int isRegisterUsed[32];
// ------------------------------- Symbol table -------------------------------



struct type_t {
    int form;
    struct object_t *fields;
    struct type_t *base;
};

typedef char *string_t;

struct object_t{
    string_t name;
    int class;
    struct type_t *type_t;
    struct object_t *next;
};

void getFromList()
{
    printf("Reading %s\n", stringValue);
}

struct object_t *objectGlobal;
struct object_t *objectLocal;

struct object_t *lastObjectGlobal;
struct object_t *lastObjectLocal;

struct type_t *typeInt;
struct type_t *typeChar;


void initTypes(){
    typeInt = malloc(sizeof(struct type_t));
    typeChar = malloc(sizeof(struct type_t));
    
    typeInt->form = FORM_INT;
    typeChar->form = FORM_CHAR;
    
}

void addToList()
{
    printf("Adding %s\n", stringValue);
}


int findTypeClassType(){
    struct object_t *tempTypeObject;
    
    tempTypeObject = malloc(sizeof(struct object_t));
    
    
    if(isGlobal == 0){
        tempTypeObject = objectLocal;
    }
    
    if(isGlobal == 1){
        tempTypeObject = objectGlobal;
    }
    
    if(tempTypeObject != 0){                                //Liste leer  => nothing to do here
        while(tempTypeObject->next != 0){
            if(strCompare(tempTypeObject->name, typeName)){
                if(isGlobal == 0){
                    lastObjectLocal->type_t = tempTypeObject->type_t;
                    return 0;
                }
                if(isGlobal == 1){
                    lastObjectGlobal->type_t = tempTypeObject->type_t;
                    return 0;
                }
            }
            tempTypeObject = tempTypeObject->next;
            
            if(strCompare(tempTypeObject->name, typeName)){
                if(isGlobal == 0){
                    lastObjectLocal->type_t = tempTypeObject->type_t;
                    return 0;
                }
                if(isGlobal == 1){
                    lastObjectGlobal->type_t = tempTypeObject->type_t;
                    return 0;
                }
            }
            
        }
    }
    return -1;

}

int findTypeClassVar(){
    struct object_t *tempTypeObject;
    struct type_t *newElement;
    
    newElement = malloc(sizeof(struct type_t));
    
    tempTypeObject = malloc(sizeof(struct object_t));
    
    
    if(isGlobal == 0){
        tempTypeObject = objectLocal;
    }
    
    if(isGlobal == 1){
        tempTypeObject = objectGlobal;
    }
    
    while (tempTypeObject->next != 0) {
        if (tempTypeObject->type_t->form == currentType) {              //Type schon in einem Object????
            if(isGlobal == 0){
                lastObjectLocal->type_t = tempTypeObject->type_t;
                return 0;
            }
            if(isGlobal == 1){
                lastObjectGlobal->type_t = tempTypeObject->type_t;
                return 0;
            }
        }
        tempTypeObject = tempTypeObject->next;
    }
    if (tempTypeObject->type_t->form == currentType) {              //Type schon in einem Object????
        if(isGlobal == 0){
            lastObjectLocal->type_t = tempTypeObject->type_t;
            return 0;
        }
        if(isGlobal == 1){
            lastObjectGlobal->type_t = tempTypeObject->type_t;
            return 0;
        }
    }
    newElement->form = currentType;
    
    tempTypeObject->type_t = newElement;

    
    return 0;
}


int addTypeToList(){
    
    struct type_t *newElement;
    struct object_t *tempTypeObject;
    
    tempTypeObject = malloc(sizeof(struct object_t));
    newElement = malloc(sizeof(struct type_t));
    

    if(isGlobal == 0){
        tempTypeObject = objectLocal;
    }
    
    if(isGlobal == 1){
        tempTypeObject = objectGlobal;
    }
    
    

    if(objectClass == 2 && (isArray == 1 || isStruct == 1)){       //Type schon vorhanden => suche nach dem struct/array
        if(findTypeClassType()){
            return 0;
        }
    }
    
    if(objectClass == 2 && (isArray == 0 && isStruct == 0)){
        if(findTypeClassVar()){
            return 0;
        }
    }
    
        
    if(objectClass == 1 && (isArray == 1 && isStruct == 0)){
        newElement->form = 3;
        newElement->base->form = currentType;
        tempTypeObject->type_t = newElement;
        return 0;
    }
    
    if(objectClass == 1 && (isArray == 0 && isStruct == 1)){
        newElement->form = 2;
        tempTypeObject->type_t = newElement;
        return 0;
    }
    
    else{
        return -1;
    }

    
    return 0;
}


int addFieldToList(){
    struct object_t *newObjectElement;
    struct object_t *newTempObject;
    
    
    newTempObject = malloc(sizeof(struct object_t));
    newObjectElement = malloc(sizeof(struct object_t));
    
    newObjectElement->name = stringValue;
    newObjectElement->class = objectClass;
    newObjectElement->next = 0;
    
    if(isGlobal == 0){
        newTempObject = lastObjectLocal->type_t->fields;
    }
    
    if(isGlobal == 1){
        newTempObject = lastObjectGlobal->type_t->fields;
    }
    
    
    
    if(newTempObject != 0){
        while (newTempObject->next != 0) {
            if(strCompare(newTempObject->name, stringValue)){
                return -1;
            }
            newTempObject = newTempObject->next;
        }
        newTempObject->next = newObjectElement;
    }
    else{
        newTempObject = newObjectElement;
        lastObjectGlobal->type_t->fields = newTempObject;
    }
    
    return 0;
}

int addObjectToList(){
    struct object_t *newObjectElement;
    struct object_t *newTempObject;
    
    newTempObject = malloc(sizeof(struct object_t));
    
    newObjectElement = malloc(sizeof(struct object_t));
    newObjectElement->name = malloc(sizeof(char) * 1024);

    strCopy(stringValue, newObjectElement->name);
    newObjectElement->class = objectClass;
    newObjectElement->next = 0;
    
    if(isGlobal == 0){
        newTempObject = objectLocal;
    }
    
    if(isGlobal == 1){
        newTempObject = objectGlobal;
    }
    
    if(newTempObject != 0){
        while (newTempObject->next != 0) {
            if(strCompare(newTempObject->name, stringValue)){
                return -1;
            }
            newTempObject = newTempObject->next;
        }
        newTempObject->next = newObjectElement;
    }
    else{
        if(isGlobal == 0){
            objectLocal = newObjectElement;
            newTempObject = newObjectElement;
        }
        if(isGlobal == 1){
            objectGlobal = newObjectElement;
            newTempObject = newObjectElement;
        }
    }
    

    
    if(isGlobal == 0){
        lastObjectLocal = newTempObject;
    }
    
    if(isGlobal == 1){
        lastObjectGlobal = newTempObject;
    }
    
    addTypeToList();
    
    return 0;
}


// -----------------------------------------------------------------------------

// -------------------- Code generation ----------------------------------------

int requestRegister()
{
    int i;
    i = 1;
    while(isRegisterUsed[i] && i <= 32)
    {
        i = i + 1;
    }
    isRegisterUsed[i] = 1;
    return i; // 32 is error
}

void releaseRegister(int i)
{
    isRegisterUsed[i] = 0;
}

int address(char identifier[1024])
{
    // TODO: Check if present in global symbol table

    // TODO: Sum of size of previous elements in symbol table.

    // TODO: Return size

    return -1;
} 

int encode(int op, int a, int b, int c)
{
    // in compiler and linker!
    // assuming: 0 <= op <= 2^6-1 = 63
    // assuming: 0 <= a <= 2^5-1 = 31
    // assuming: 0 <= b <= 2^5-1 = 31
    // assuming: -32768 = -2^15 <= c <= 2^26-1 = 67108863
    // assuming: if c > 2^15-1 = 32767 then a == 0 and b == 0
    if (c < 0)
    {
        c = c + 65536; // 0x10000: 2^16
    }
    // if << is not available
    // replace (x << 5) by (x * 32) and (x << 16) by (x * 65536)
    return (((((op * 32) + a) * 32) + b) * 65536) + c;
}
// -----------------------------------------------------------------------------

// --------------------Parser error reporting ----------------------------------

void fail(char message[1024])
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

    printf("Fail near Line %d: %s\n", niceLine, message);
    exit(-1);
}

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

    printf("Error near Line %d: %s\n", niceLine, message);
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

int isIn(int tokenType, int rule) {

    if(rule == FIRST_TOP_DECLARATION && (
        tokenType == TOKEN_TYPEDEF ||
        tokenType == TOKEN_STRUCT ||
        tokenType == TOKEN_STATIC ||
        isIn(tokenType, FIRST_TYPE) ))
    { return 1; }

    if(rule == FIRST_TYPE_DECLARATION && (
        tokenType == TOKEN_STRUCT ||
        tokenType == TOKEN_TYPEDEF))
    { return 1; }

    if(rule == FIRST_EXPRESSION && (
        tokenType == TOKEN_MINUS ||
        tokenType == TOKEN_IDENTIFIER ||
        tokenType == TOKEN_CONSTINT ||
        tokenType == TOKEN_CONSTCHAR ||
        tokenType == TOKEN_STRING_LITERAL ||
        tokenType == TOKEN_LRB ||
        tokenType == TOKEN_FCLOSE ||
        tokenType == TOKEN_FOPEN ||
        tokenType == TOKEN_SIZEOF ||
        tokenType == TOKEN_MALLOC))
    { return 1; }

    if(rule == FIRST_TYPE && (
        tokenType == TOKEN_INT ||
        tokenType == TOKEN_CHAR ||
        tokenType == TOKEN_VOID ||
        tokenType == TOKEN_STRUCT ))
    { return 1; }

    if(rule == FIRST_VARIABLE_DECLARATION && (
        tokenType == TOKEN_STATIC ||
        isIn(tokenType, FIRST_TYPE) ))
    { return 1; }

    if(rule == FIRST_INSTRUCTION && (
        isIn(tokenType, FIRST_VARIABLE_DECLARATION) ||
        isIn(tokenType, FIRST_TYPE_DECLARATION) ||
        tokenType == TOKEN_IDENTIFIER))
    { return 1; }

    return 0;
}

// -------------------------- EBNF --------------------------------------------

void instruction();
void if_else();
void while_loop();
void expression();
void type_declaration();

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
    
    if(tokenType == TOKEN_IDENTIFIER)
    {
        // TODO: Get from list

        getNextToken();
        return;
    }

    if(tokenType == TOKEN_STRUCT)
    {
        getNextToken();
        if(tokenType == TOKEN_IDENTIFIER)
        {
            tokenType = TOKEN_STRUCT;
            strCopy(stringValue, typeName);

            // TODO: Check if valid type

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
    if(tokenType == TOKEN_LRB)
    {
        getNextToken();
        expression();

        if(tokenType == TOKEN_RRB)
        {
            getNextToken();
        }
        else
        {
            mark(") missing (factor)");
            getNextToken();
        }

        return;
    }

    if(tokenType == TOKEN_CONSTINT)
    {
        // TODO: Handle integer literal
        getNextToken();
        return;
    }

    if(tokenType == TOKEN_CONSTCHAR)
    {
        // TODO: Handle char literal
        getNextToken();
        return;
    }

    if(tokenType == TOKEN_IDENTIFIER) // not sure if call or variable
    {
        // TODO: save identifier
        getNextToken();

        if(tokenType == TOKEN_LRB) // procedure call
        {
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
                mark(") expected (factor)");
                getNextToken();
            }

            if(tokenType == TOKEN_SEMICOLON)
            {
                getNextToken();
            }
            else
            {
                mark("; expected after expression (factor)");
                getNextToken();
            }

            return;
        }

        while(tokenType == TOKEN_ACCESS)
        {
            getNextToken();
            if(tokenType == TOKEN_IDENTIFIER)
            {
                // TODO: store identifier
                getNextToken();
            }
            else
            {
                error("Identifier expected (factor)");
                return;
            }
        }

        if(tokenType == TOKEN_LSB)
        {
            getNextToken();
            if(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression();
            }
            else
            {
                error("expression expected (factor)");
            }

            if(tokenType == TOKEN_RSB)
            {
                getNextToken();
            }
            else
            {
                mark("] expected (factor)");
                getNextToken();
            }

            // TODO: Handle array access

            return;
        }

        // TODO: Handle variable access

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

void variable_declaration()
{
    if(tokenType == TOKEN_STATIC)
    {
        // TODO: Handle static?
        getNextToken();
    }

    if(isIn(tokenType, FIRST_TYPE))
    {
        type();

        while(tokenType == TOKEN_MULT)
        {
            //isArray = 1;
            getNextToken();
        }

        if(tokenType == TOKEN_IDENTIFIER)
        {
            // TODO: Add to symbol table (either global or local)
            //         if(addObjectToListGlobal() < 0) 
            //         {
            //             fail("Double declaration of variable");
            //         }
            //         isArray = 0; // default value

            getNextToken();
        }
        else
        {
            error("Identifier expected (variable_declaration)");
        }
    }
    else
    {
        error("Type expected (variable_declaration)");
    }
}

void return_statement()
{
    if(tokenType == TOKEN_RETURN)
    {
        getNextToken();

        if(isIn(tokenType, FIRST_EXPRESSION))
        {
            expression();
        }
    }
}

void instruction()
{
    if(tokenType == TOKEN_IF)
    {
        if_else();
        return;
    }

    if(tokenType == TOKEN_WHILE)
    {
        while_loop();
        return;
    }

    if(tokenType == TOKEN_RETURN)
    {
        return_statement();
        if(tokenType == TOKEN_SEMICOLON)
        {
            getNextToken();
        }
        else
        {
            mark("; expected after return statement (instruction)");
            getNextToken();
        }

        return;
    }

    // // Don't support type declarations within functions
    // if(isIn(tokenType, FIRST_TYPE_DECLARATION))
    // {
    //     type_declaration();
    //     if(tokenType == TOKEN_SEMICOLON)
    //     {
    //         getNextToken();
    //     }
    //     else
    //     {
    //         mark("; expected after type declaration (instruction)");
    //         getNextToken();
    //     }

    //     return;
    // }

    if(tokenType == TOKEN_IDENTIFIER) // assignment or call or typedefd declaration
    {
        // TODO: Save identifier
        getNextToken();

        if(tokenType == TOKEN_IDENTIFIER) // typedefd declaration
        {
            // TODO: Add to list
            getNextToken();

            if(tokenType == TOKEN_SEMICOLON)
            {
                getNextToken();
            }
            else
            {
                mark("; expected after declaration (instruction)");
                getNextToken();
            }

        }

        if(tokenType == TOKEN_LRB) // procedure call
        {
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
                mark(") expected (instruction)");
                getNextToken();
            }

            if(tokenType == TOKEN_SEMICOLON)
            {
                getNextToken();
            }
            else
            {
                mark("; expected after expression (instruction)");
                getNextToken();
            }

            return;
        }

        while(tokenType == TOKEN_ACCESS)
        {
            getNextToken();
            if(tokenType == TOKEN_IDENTIFIER)
            {
                // TODO: store identifier
                getNextToken();
            }
            else
            {
                error("Identifier expected (factor)");
                return;
            }
        }

        if(tokenType == TOKEN_LSB)
        {
            getNextToken();
            if(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression();
            }
            else
            {
                error("expression expected (factor)");
            }

            if(tokenType == TOKEN_RSB)
            {
                getNextToken();
            }
            else
            {
                mark("] expected (factor)");
                getNextToken();
            }
        }

        if(tokenType == TOKEN_ASSIGNMENT) // assignment
        {
            getNextToken();
            expression();
            if(tokenType == TOKEN_SEMICOLON)
            {
                getNextToken();
            }
            else
            {
                mark("; expected after expression (instruction)");
                getNextToken();
            }

            return;
        }
    }

    if(isIn(tokenType, FIRST_VARIABLE_DECLARATION))
    {
        variable_declaration();

        isGlobal = 0;
        objectClass = 2; // TODO: Magic int for VAR
        addObjectToList();

        if(tokenType == TOKEN_SEMICOLON)
        {
            getNextToken();
        }
        else
        {
            mark("; expected after variable declaration (instruction)");
            getNextToken();
        }

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
        }
        else
        {
            mark("( expected (if_else)");
            getNextToken();
        }

        if(isIn(tokenType, FIRST_EXPRESSION))
        {
            expression();

            if(tokenType == TOKEN_LRB)
            {
                getNextToken();
            }
            else
            {
                mark(") expected (if_else)");
                getNextToken();
            }

            if(tokenType == TOKEN_LCB)
            {
                getNextToken();
            }
            else
            {
                mark("{ expected (if_else)");
                getNextToken();
            }

            while(isIn(tokenType, FIRST_INSTRUCTION))
            {
                instruction();
            }

            if(tokenType == TOKEN_RCB)
            {
                getNextToken();
            }
            else
            {
                mark("} expected (if_else)");
                getNextToken();
            }

            if(tokenType == TOKEN_ELSE)
            {
                if(tokenType == TOKEN_LCB)
                {
                    getNextToken();
                }
                else
                {
                    mark("{ expected in else branch (if_else)");
                    getNextToken();
                }

                while(isIn(tokenType, FIRST_INSTRUCTION))
                {
                    instruction();
                }

                if(tokenType == TOKEN_RCB)
                {
                    getNextToken();
                }
                else
                {
                    mark("} expected in else branch (if_else)");
                    getNextToken();
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
        }
        else
        {
            mark("( expected (while_loop)");
            getNextToken();
        }

        if(isIn(tokenType, FIRST_EXPRESSION))
        {
            expression();

            if(tokenType == TOKEN_LRB)
            {
                getNextToken();
            }
            else
            {
                mark(") expected (while_loop)");
                getNextToken();
            }

            if(tokenType == TOKEN_LCB)
            {
                getNextToken();
            }
            else
            {
                mark("{ expected (while_loop)");
                getNextToken();
            }

            while(isIn(tokenType, FIRST_INSTRUCTION))
            {
                instruction();
            }

            if(tokenType == TOKEN_RCB)
            {
                getNextToken();
            }
            else
            {
                mark("} expected (while_loop)");
                getNextToken();
            }
        }
        else
        {
            error("expression expected (while_loop)");
        }
    }
}

void function_declaration()
{
    if(isIn(tokenType, FIRST_TYPE))
    {
        getNextToken();

        if(tokenType == TOKEN_IDENTIFIER)
        {
            // TODO: Handle procedure
            getNextToken();

            if(tokenType == TOKEN_LRB)
            {
                getNextToken();
            }
            else
            {
                mark("( expected (function_declaration)");
                getNextToken();
            }

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
            }
            else
            {
                mark(") expected (function_declaration)");
                getNextToken();
            }

            if(tokenType == TOKEN_SEMICOLON)
            {
                // TODO: Handle function prototype
                getNextToken();
                return;
            }

            if(tokenType == TOKEN_LCB)
            {
                getNextToken();
            }
            else
            {
                mark("{ expected (function_declaration)");
                getNextToken();
            }

            while(isIn(tokenType, FIRST_INSTRUCTION))
            {
                instruction();
            }

            if(tokenType == TOKEN_RCB)
            {
                getNextToken();
            }
            else
            {
                mark("} expected (function_declaration)");
                getNextToken();
            }

        }
        else
        {
            error("Identifier expected (function_declaration)");
        }
    }
    else
    {
        error("Type expected (function_declaration)");
    }
}

void typedef_declaration()
{
    if(tokenType == TOKEN_TYPEDEF)
    {
        getNextToken();

        if(isIn(tokenType, FIRST_TYPE))
        {
            type();
            while(tokenType == TOKEN_MULT)
            {
                // TODO: Handle references
                getNextToken();
            }

            if(tokenType == TOKEN_IDENTIFIER)
            {
                // TODO: Add type to symbol table
                getNextToken();
            }
            else
            {
                error("Identifier expected (typedef_declaration)");
            }
        }
        else
        {
            error("type expected (typedef_declaration)");
        }
    }
    else
    {
        error("typedef expected");
    }
}

void struct_declaration()
{
    if(tokenType == TOKEN_STRUCT)
    {
        
        getNextToken();

        if(tokenType == TOKEN_IDENTIFIER)
        {
            isStruct = 1;
            objectClass = CLASS_TYPE; 
            isArray = 0;
            isGlobal = 1;
            addObjectToList();
            
            getNextToken();

            if(tokenType == TOKEN_LCB)
            {
                getNextToken();
            }
            else
            {
                mark("{ expected (struct_declaration)");
                getNextToken();
            }

            while(isIn(tokenType, FIRST_VARIABLE_DECLARATION)) {
                variable_declaration();

                // type is set by type() within variable_declaration().
                // identifier name is still set after variable_declaration() completed.
                objectClass = CLASS_FIELD; // TODO: Magic int
                addFieldToList();

                if(tokenType == TOKEN_SEMICOLON)
                {
                    getNextToken();
                }
                else
                {
                    mark("; expected after variable declaration (struct_declaration)");
                    getNextToken();
                }
            }

            if(tokenType == TOKEN_RCB)
            {
                getNextToken();
            }
            else
            {
                mark("} expected (struct_declaration)");
                getNextToken();
            }
        }
        else
        {
            error("Identifier expected (struct_declaration)");
        }
    }
}

void type_declaration()
{
    if(tokenType == TOKEN_STRUCT)
    {
        struct_declaration();
        return;
    }

    if(tokenType == TOKEN_TYPEDEF)
    {
        typedef_declaration();
        return;
    }
}

void top_declaration() {

    if(isIn(tokenType, FIRST_TYPE_DECLARATION))
    {
        type_declaration();

        if(tokenType == TOKEN_SEMICOLON)
        {
            getNextToken();
        }
        else
        {
            mark("; expected after type declaration (top_declaration)");
            getNextToken();
        }
        return;
    }

    if(tokenType == TOKEN_STATIC)
    {
        variable_declaration();

        if(tokenType == TOKEN_SEMICOLON)
        {
            getNextToken();
        }
        else
        {
            mark("; expected after variable declaration (top_declaration)");
            getNextToken();
        }
        return;
    }

    if(isIn(tokenType, FIRST_TYPE))
    {
        function_declaration();

        return;
    }
}

void include_def()
{
    if(tokenType == TOKEN_INCLUDE)
    {
        getNextToken();
        if(tokenType == TOKEN_STRING_LITERAL)
        {
            // TODO: Handle include

            getNextToken();
        }
        else
        {
            error("Expected string literal (start)");
        }
    }
}

void start() {
    getNextToken();
    while(tokenType == TOKEN_INCLUDE)
    {
        include_def();
    }

    while(tokenType != TOKEN_EOF) {
        if(isIn(tokenType, FIRST_TOP_DECLARATION))
        {
            top_declaration();
        }
        else
        {
            getNextToken();
        }
        
    }
}
// ----------------------------------------------------------------------------

int main(){
    printf("\nPhoenix: Parser\n===============\n");
    initTokens();
    initTypes();

    errorCount = 0;
    warningCount = 0;
    tokenType = -1;
    openFile("test/m4.c");
    //openFile("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/test/m4.c");
    start();
    printf("Parsed with %d errors, %d warnings\n", errorCount, warningCount);

    return 0;
}
