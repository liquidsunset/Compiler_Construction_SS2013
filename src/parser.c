#include <stdlib.h>
#include "scanner.c"

static int currentType;    //2 = int, 3 = char
static char typeName[1024]; //name from struct or array
static int isArray;
static int isStruct;
static int isGlobal; // 0 for local, 1 for global
static int objectClass;
static int lastOffsetPointer;

// -- Codegen
static int SIZE_INT;

static int isRegisterUsed[32];
static int CODEGEN_GP;
static int CODEGEN_MODE_CONST;
static int CODEGEN_MODE_VAR;
static int CODEGEN_MODE_REG;
static int CODEGEN_MODE_REF;

// ------------------------------- Symbol table -------------------------------

struct object_t;

struct type_t {
    int form;
    int size;
    struct object_t *fields;
    struct type_t *base;
};

typedef char *string_t;

struct object_t{
    string_t name;
    int class;
    int offset;
    struct type_t *type;
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

struct object_t *lastFieldElementGlobal;
struct object_t *lastFieldElementLocal;

struct type_t *typeInt;
struct type_t *typeChar;


void initTypes(){
    typeInt = malloc(sizeof(struct type_t));
    typeChar = malloc(sizeof(struct type_t));
    
    typeInt->form = FORM_INT;
    typeInt->size = 4;
    typeChar->form = FORM_CHAR;
    typeChar->size = 4;
    lastOffsetPointer = 0;
    
}

void addToList()
{
    printf("Adding %s\n", stringValue);
}


struct object_t *findObject(struct object_t *firstElement){
    
    if(firstElement != 0){
        while (firstElement->next != 0) {
            if(strCompare(firstElement->name, stringValue)){
                printf("\nSymbol table: found %s\n", stringValue);
                return firstElement;
            }
            firstElement = firstElement->next;
        }
        if(strCompare(firstElement->name, stringValue)){
            printf("\nSymbol table: found %s\n", stringValue);
            return firstElement;
        }
    }
    
    return 0;
}

struct type_t *findType(){
    return 0;
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
                    lastObjectLocal->type = tempTypeObject->type;
                    return 0;
                }
                if(isGlobal == 1){
                    lastObjectGlobal->type = tempTypeObject->type;

                }
            }
            tempTypeObject = tempTypeObject->next;
            
            if(strCompare(tempTypeObject->name, typeName)){
                if(isGlobal == 0){
                    lastObjectLocal->type = tempTypeObject->type;
                    return 0;
                }
                if(isGlobal == 1){
                    lastObjectGlobal->type = tempTypeObject->type;
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
        if (tempTypeObject->type->form == currentType) {              //Type schon in einem Object????
            if(isGlobal == 0){
                lastObjectLocal->type = tempTypeObject->type;
                return 0;
            }
            if(isGlobal == 1){
                lastObjectGlobal->type = tempTypeObject->type;
                return 0;
            }
        }
        tempTypeObject = tempTypeObject->next;
    }
    if (tempTypeObject->type->form == currentType) {              //Type schon in einem Object????
        if(isGlobal == 0){
            lastObjectLocal->type = tempTypeObject->type;
            return 0;
        }
        if(isGlobal == 1){
            lastObjectGlobal->type = tempTypeObject->type;
            return 0;
        }
    }
    newElement->form = currentType;
    
    tempTypeObject->type = newElement;

    
    return 0;
}


int addTypeToList(){
    
    struct type_t *newElement;
    struct object_t *tempTypeObject;
    
    tempTypeObject = malloc(sizeof(struct object_t));
    newElement = malloc(sizeof(struct type_t));
    

    if(isGlobal == 0){
        tempTypeObject = lastObjectLocal;
    }
    
    if(isGlobal == 1){
        tempTypeObject = lastObjectGlobal;
    }
    
    

    if(objectClass == 2 && (isArray == 1 || isStruct == 1)){       //Type schon vorhanden => suche nach dem struct/array
        if(findTypeClassType()){
            tempTypeObject->offset = lastOffsetPointer;
            lastOffsetPointer = lastOffsetPointer - 4;
            return 0;
        }
    }
    
    if(objectClass == 2 && (isArray == 0 && isStruct == 0)){
        if(currentType == FORM_INT){
            tempTypeObject->type = typeInt;
            //tempTypeObject->type->size = 4;
        }
        if(currentType == FORM_CHAR){
            tempTypeObject->type = typeChar;
            //tempTypeObject->type->size = 4;
        }
        
        tempTypeObject->offset = lastOffsetPointer - 4;
        lastOffsetPointer = lastOffsetPointer - 4;
        
        return 0;
    }
    
        
    if(objectClass == 1 && (isArray == 1 && isStruct == 0)){
        newElement->form = FORM_ARRAY;
        if(currentType == FORM_INT){
            newElement->base = typeInt;
        }
        if (currentType == FORM_CHAR) {
            newElement->base = typeChar;
        }
        
        newElement->base->form = currentType;
        tempTypeObject->type = newElement;
        
        return 0;
    }
    
    if(objectClass == CLASS_TYPE && (isArray == 0 && isStruct == 1)){
        newElement->form = FORM_RECORD;
        newElement->size = 0;
        tempTypeObject->type = newElement;
        return 0;
    }
    
    else{
        return -1;
    }

    
    return 0;
}

int addTypeToField(){
    
    if(currentType == FORM_INT && isArray == 1 && isStruct == 0){
        struct type_t *newType;
        newType = malloc(sizeof(struct type_t));
        newType->size = 4;
        newType->form = FORM_ARRAY;
        newType->base = typeInt;
        
        if(isGlobal == 0){
            lastFieldElementLocal->type = newType;
            lastObjectLocal->type->size = lastObjectLocal->type->size + 4;
        }
        if(isGlobal == 1){
            lastFieldElementGlobal->type = newType;
            lastObjectGlobal->type->size = lastObjectGlobal->type->size + 4;
        }
        return 0;
        
    }

    
    if(currentType == FORM_INT ){
        if(isGlobal == 0){
            lastFieldElementLocal->type = typeInt;
            //lastFieldElementLocal->type->size = 4;
            lastObjectLocal->type->size = lastObjectLocal->type->size + 4;
        }
        if(isGlobal == 1){
            lastFieldElementGlobal->type = typeInt;
            //lastFieldElementGlobal->type->size = 4;
            lastObjectGlobal->type->size = lastObjectGlobal->type->size + 4;
        }
    }
    
    if(currentType == FORM_CHAR){
        if(isGlobal == 0){
            lastFieldElementLocal->type = typeChar;
            //lastFieldElementLocal->type->size = 4;
            lastObjectLocal->type->size = lastObjectLocal->type->size + 4;
        }
        if(isGlobal == 1){
            lastFieldElementGlobal->type = typeChar;
            //lastFieldElementGlobal->type->size = 4;
            lastObjectGlobal->type->size = lastObjectGlobal->type->size + 4;
        }
    }
    
    return 0;
    
}


int addFieldToList(){
    struct object_t *newObjectElement;
    struct object_t *newTempObject;
    
    
    newTempObject = malloc(sizeof(struct object_t));
    newObjectElement = malloc(sizeof(struct object_t));
    newObjectElement->name = malloc(sizeof(char) * 1024);
    
    
    strCopy(stringValue, newObjectElement->name);
    newObjectElement->class = objectClass;
    newObjectElement->next = 0;

    if(isGlobal == 0){
        newTempObject = lastObjectLocal->type->fields;
    }
    
    if(isGlobal == 1){
        newTempObject = lastObjectGlobal->type->fields;
    }
    
    //TODO: implementing Struct in Struct and Array in Struct
    //irgendwos is scho erledigt oder so kp  ho an ueberlick verlorn^^
    
    
    if(newTempObject != 0){
        while (newTempObject->next != 0) {
            if(strCompare(newTempObject->name, stringValue)){
                return -1;
            }
            newTempObject = newTempObject->next;
        }
        newTempObject->next = newObjectElement;
        lastFieldElementGlobal = newTempObject->next;
    }
    else{
        newTempObject = newObjectElement;
        lastObjectGlobal->type->fields = newTempObject;
        lastFieldElementGlobal = newTempObject;
    }
    
    addTypeToField();
    
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
        lastObjectLocal = newObjectElement;
    }
    
    if(isGlobal == 1){
        lastObjectGlobal = newObjectElement;
    }
    
    addTypeToList();
    
    return 0;
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

    printf("\nFail near Line %d: %s\n", niceLine, message);
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

    printf("\nError near Line %d: %s\n", niceLine, message);
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

    printf("\nWarning Near Line %d: %s\n", niceLine, message);
}
// -----------------------------------------------------------------------------


// -------------------- Code generation ----------------------------------------

void initCodeGen()
{
    CODEGEN_GP = 28;

    SIZE_INT = 4;

    CODEGEN_MODE_CONST = 1;
    CODEGEN_MODE_VAR = 2;
    CODEGEN_MODE_REG = 3;
    CODEGEN_MODE_REF = 4;
}

struct item_t
{
    int mode; // CONST, VAR, REG, REF
    struct type_t * type;
    int reg; // both reg and offset give us the address of the variable
    int offset; // reg[reg] + offset -> address
    int value;
};

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

void storeString(struct item_t * item, char string[1024])
{
    // TODO
}

void put(int op, int a, int b, int c)
{
    int instruction;

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
    instruction = (((((op * 32) + a) * 32) + b) * 65536) + c;

    // TODO: write to file
}

void ref2Reg(struct item_t * item)
{
    item->mode = CODEGEN_MODE_REG;
    put(TARGET_LW, item->reg, item->reg, item->offset);
    item->offset = 0;
}

void const2Reg(struct item_t * item)
{
    item->mode = CODEGEN_MODE_REG;
    item->reg = requestRegister();

    put(TARGET_ADDI, item->reg, 0, item->value);

    item->value = 0;
    item->offset = 0;
}

void var2Reg(struct item_t * item)
{
    int newReg;
    
    item->mode = CODEGEN_MODE_REG;
    newReg = requestRegister();

    put(TARGET_LW, newReg, item->reg, item->offset);

    item->reg = newReg;
    item->offset = 0;
}

void load(struct item_t * item)
{
    if(item->mode == CODEGEN_MODE_CONST)
    {
        const2Reg(item);
        return;
    }
    if(item->mode == CODEGEN_MODE_VAR)
    {
        var2Reg(item);
        return;
    }
    if(item->mode == CODEGEN_MODE_REF)
    {
        ref2Reg(item);
        return;
    }
}

void assignmentOperator(
    struct item_t * leftItem,
    struct item_t * rightItem)
{
    if((leftItem->type->form == FORM_INT) && (leftItem->type != rightItem->type))
    {
        mark("Types of assignment dont match");
    }
    // leftItem must be in VAR mode
    load(rightItem); 
    put(TARGET_SW, rightItem->reg, leftItem->reg, leftItem->offset);
    releaseRegister(rightItem->reg); 
}

void termBinaryOperator(
    struct item_t * leftItem,
    struct item_t * rightItem,
    int operatorSymbol)
{
    if(leftItem->type == rightItem->type)
    {
        if(rightItem->mode == CODEGEN_MODE_CONST)
        {
            if(leftItem->mode == CODEGEN_MODE_CONST)
            {
                if(operatorSymbol == TOKEN_MULT)
                {
                    leftItem->value = leftItem->value * rightItem->value;
                }
                else{if(operatorSymbol == TOKEN_DIVIDE)
                {
                    leftItem->value = leftItem->value / rightItem->value;
                }}
            }
            else
            {
                load(leftItem);
                if(operatorSymbol == TOKEN_MULT)
                {
                    put(TARGET_MULI, leftItem->reg, leftItem->reg, rightItem->value);
                }
                else{if(operatorSymbol == TOKEN_DIVIDE){
                    put(TARGET_DIVI, leftItem->reg, leftItem->reg, rightItem->value);
                }}
            }
        }
        else
        {
            load(leftItem);
            load(rightItem);
            if(operatorSymbol == TOKEN_MULT)
            {
                put(TARGET_MUL, leftItem->reg, leftItem->reg, rightItem->reg);
            }
            else {if(operatorSymbol == TOKEN_DIVIDE){
                put(TARGET_DIV, leftItem->reg, leftItem->reg, rightItem->reg);
            }}
            releaseRegister(rightItem->reg);
        }
    }
    else
    {
        error("Types dont match");
    }

}

void simpleExpressionBinaryOperator(
    struct item_t * leftItem,
    struct item_t * rightItem,
    int operatorSymbol)
{
    if(operatorSymbol == TOKEN_OR)
    {
        // .. later
        return;
    }
    if(leftItem->type == typeInt && rightItem->type == typeInt) 
    {
        if(rightItem->mode == CODEGEN_MODE_CONST)
        {
            if(leftItem->mode == CODEGEN_MODE_CONST)
            {
                if(operatorSymbol == TOKEN_PLUS)
                {
                    leftItem->value = leftItem->value + rightItem->value;
                }
                else{if(operatorSymbol == TOKEN_MINUS)
                {
                    leftItem->value = leftItem->value - rightItem->value;
                }}
            }
            else
            {
                load(leftItem);
                if(operatorSymbol == TOKEN_PLUS)
                {
                    put(TARGET_ADDI, leftItem->reg, leftItem->reg, rightItem->value);
                }
                else{if(operatorSymbol == TOKEN_MINUS){
                    put(TARGET_SUBI, leftItem->reg, leftItem->reg, rightItem->value);
                }}
            }
        }
        else
        {
            load(leftItem);
            load(rightItem);
            if(operatorSymbol == TOKEN_PLUS)
            {
                put(TARGET_ADD, leftItem->reg, leftItem->reg, rightItem->reg);
            }
            else {if(operatorSymbol == TOKEN_MINUS){
                put(TARGET_SUB, leftItem->reg, leftItem->reg, rightItem->reg);
            }}
            releaseRegister(rightItem->reg);
        }
    }
    else
    {
        error("Integer expression expected");
    }
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

    if(rule == FIRST_BASIC_TYPES && (
        tokenType == TOKEN_INT ||
        tokenType == TOKEN_CHAR))
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
void expression(struct item_t * item);
void type_declaration();
void selector(struct item_t * item);

void type(struct item_t * item)
{
    struct object_t * object;

    if(tokenType == TOKEN_INT)
    {
        getNextToken();

        isArray = 0;
        isStruct = 0;

        currentType = TOKEN_INT;
        item->mode = CODEGEN_MODE_CONST;
        item->type = typeInt;
        item->reg = 0;
        item->offset = 0;
        item->value = SIZE_INT;
        
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
        object = findObject(objectGlobal);

        if(object == 0)
        {
            error("Unknown type");
        }
        getNextToken();
        return;
    }

    if(tokenType == TOKEN_STRUCT)
    {
        getNextToken();

        isArray = 0;
        isStruct = 1;

        if(tokenType == TOKEN_IDENTIFIER)
        {
            currentType = TOKEN_STRUCT;
            strCopy(stringValue, typeName);

            object = findObject(objectGlobal);

            item->mode = CODEGEN_MODE_CONST;
            item->type = typeInt;
            item->reg = 0;
            item->offset = 0;
            item->value = object->type->size;

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

void sizeof_func(struct item_t * item)
{
    if(tokenType == TOKEN_SIZEOF)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            
            if(isIn(tokenType, FIRST_TYPE))
            {
                type(item);
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

void malloc_func(struct item_t * item)
{
    if(tokenType == TOKEN_MALLOC)
    {
        getNextToken();
        if(tokenType == TOKEN_LRB)
        {
            getNextToken();
            
            if(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression(item);
                if(tokenType == TOKEN_RRB)
                {
                    getNextToken();
                    if(item->mode == CODEGEN_MODE_CONST) // only allow allocation known during compile time
                    {
                        load(item);
                        put(TARGET_MALLOC, item->reg, 0, item->reg);
                    }
                    else
                    {
                        error("Invalid allocation");
                    }
                }
                else
                {
                    mark(") expected (malloc)");
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
                            mark(") expected (fopen_func)");
                            getNextToken();
                        }
                    }
                    else
                    {
                        mark("File mode expected (fopen_func)");
                        getNextToken();
                    }
                }
                else
                {
                    mark("Comma expected (fopen_func)");
                    getNextToken();
                }
            }
            else
            {
                mark("Identifier expected (fopen_func)");
                getNextToken();
            }
        }
        else
        {
            mark("( expected (fopen_func)");
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
                    mark(") expected (fclose_func)");
                    getNextToken();
                }
            }
            else
            {
                mark("Identifier expected (fclose_func)");
                getNextToken();
            }
        }
        else
        {
            mark("( expected (fclose_func)");
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
                    mark(") expected (fgetc_func)");
                    getNextToken();
                }
            }
            else
            {
                mark("Identifier expected (fgetc_func)");
                getNextToken();
            }
        }
        else
        {
            mark("( expected (fgetc_func)");
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
                    mark(") expected (fputc_func)");
                    getNextToken();
                }
            }
            else
            {
                mark("Identifier expected (fputc_func)");
                getNextToken();
            }
        }
        else
        {
            mark("( expected (fputc_func)");
            getNextToken();
        }
    }
}

void factor(struct item_t * item) {
    struct object_t * object;

    if(tokenType == TOKEN_LRB)
    {
        getNextToken();
        expression(item);

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
        item->mode = CODEGEN_MODE_CONST;
        item->type = typeInt;
        item->reg = 0;
        item->offset = 0;
        item->value = intValue;

        getNextToken();
        return;
    }

    if(tokenType == TOKEN_STRING_LITERAL)
    {
        storeString(item, stringValue);
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
        object = findObject(objectGlobal); // implicitly uses stringValue
        getNextToken();
        if(object != 0)
        {
            item->mode = CODEGEN_MODE_VAR;
            item->type = object->type;

            // TODO: distinction between global and local scope
            item->reg = CODEGEN_GP;

            item->offset = object->offset;

            selector(item); // array, record access
        }
        else
        {
            error("Undeclared variable");
            return;
        }

        // // We do not support procedure calls now
        // if(tokenType == TOKEN_LRB) // procedure call
        // {
        //     while(isIn(tokenType, FIRST_EXPRESSION))
        //     {
        //         expression(0);
        //         if(tokenType == TOKEN_COMMA)
        //         {
        //             getNextToken();
        //         }
        //     }

        //     if(tokenType == TOKEN_RRB)
        //     {
        //         getNextToken();
        //     }
        //     else
        //     {
        //         mark(") expected (factor)");
        //         getNextToken();
        //     }

        //     if(tokenType == TOKEN_SEMICOLON)
        //     {
        //         getNextToken();
        //     }
        //     else
        //     {
        //         mark("; expected after expression (factor)");
        //         getNextToken();
        //     }

        //     return;
        // }
        return;
    }

    if(tokenType == TOKEN_SIZEOF)
    {
        sizeof_func(item);
        return;
    }

    if(tokenType == TOKEN_MALLOC)
    {
        malloc_func(item);
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
        expression(item);
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

void term(struct item_t * item)
{
    int operatorSymbol;
    struct item_t * rightItem;

    factor(item);

    while(tokenType == TOKEN_MULT || tokenType == TOKEN_DIVIDE)
    {
        operatorSymbol = tokenType;
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        factor(rightItem);
        termBinaryOperator(item, rightItem, operatorSymbol);
    }
}

void simple_expression(struct item_t * item)
{
    int operatorSymbol;
    //struct item_t * leftItem;
    struct item_t * rightItem;

    if(tokenType == TOKEN_MINUS)
    {
        getNextToken();
    }

    //leftItem = malloc(sizeof(struct item_t));
    //term(leftItem);
    term(item);

    while(tokenType == TOKEN_PLUS || tokenType == TOKEN_MINUS)
    {
        operatorSymbol = tokenType;
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        term(rightItem);
        simpleExpressionBinaryOperator(item, rightItem, operatorSymbol);
    }
}

void expression(struct item_t * item)
{
    if(tokenType == TOKEN_NOT) {
        getNextToken();
    }

    simple_expression(item);
    
    if(tokenType == TOKEN_EQUAL)
    {
        getNextToken();
        expression(0);
        return;
    }
    if(tokenType == TOKEN_LESSEQUAL)
    {
        getNextToken();
        expression(0);
        return;
    }
    if(tokenType == TOKEN_LESS)
    {
        getNextToken();
        expression(0);
        return;
    }
    if(tokenType == TOKEN_UNEQUAL)
    {
        getNextToken();
        expression(0);
        return;
    }
    if(tokenType == TOKEN_GREATER)
    {
        getNextToken();
        expression(0);
        return;
    }
    if(tokenType == TOKEN_GREATEREQUAL)
    {
        getNextToken();
        expression(0);
        return;
    }
    if(tokenType == TOKEN_AND)
    {
        getNextToken();
        expression(0);
        return;
    }
    if(tokenType == TOKEN_OR)
    {
        getNextToken();
        expression(0);
        return;
    }
    // if(tokenType == TOKEN_ASSIGNMENT)
    // {
    //     getNextToken();
    //     expression(0);
    //     return;
    // }
}

void variable_declaration()
{
    struct item_t * item;

    if(tokenType == TOKEN_STATIC)
    {
        // TODO: Handle static?
        getNextToken();
    }

    if(isIn(tokenType, FIRST_TYPE))
    {
        item = malloc(sizeof(struct item_t));
        type(item);

        if(tokenType == TOKEN_MULT)
        {
            isArray = 1;
            isStruct = 0;
            getNextToken();
        }

        if(tokenType == TOKEN_IDENTIFIER)
        {
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

void return_statement(struct item_t * item)
{
    if(tokenType == TOKEN_RETURN)
    {
        getNextToken();

        if(isIn(tokenType, FIRST_EXPRESSION))
        {
            expression(item);
        }
    }
}

void field(
    struct item_t * item,
    struct object_t * object)
{
    load(item);
    item->mode = CODEGEN_MODE_REF;
    item->offset = object->offset;
    item->type = object->type;
}

void index(
    struct item_t * item,
    struct item_t * indexItem)
{
    if(indexItem->mode == CODEGEN_MODE_CONST)
    {
        load(item);
        item->mode = CODEGEN_MODE_REF;
        item->offset = indexItem->value * 4; // we only use basic types or references of size of 4 bytes
    }
    else
    {
        load(indexItem);
        put(TARGET_MULI, indexItem->reg, indexItem->reg, 4);
        load(item);
        item->mode = CODEGEN_MODE_REF;
        put(TARGET_ADD, item->reg, item->reg, indexItem->reg);
        releaseRegister(indexItem->reg);
    }
    item->type = item->type->base;
}

void selector(struct item_t * item)
{
    struct object_t * object;
    struct item_t * indexItem;

    while((tokenType == TOKEN_ACCESS) || (tokenType == TOKEN_LSB))
    {
        if(tokenType == TOKEN_ACCESS)
        {
            getNextToken();
            if(tokenType == TOKEN_IDENTIFIER)
            {
                object = findObject(item->type->fields);
                if(object != 0)
                {
                    field(item, object);
                }
                else
                {
                    error("Unknown field (selector)");
                }
                getNextToken();
            }
            else
            {
                error("Identifier expected (selector)");
            }

        }
        else {if(tokenType == TOKEN_LSB){
            getNextToken();
            indexItem = malloc(sizeof(struct item_t));
            expression(indexItem);
            index(item, indexItem);
            if(tokenType == TOKEN_RSB)
            {
                getNextToken();
            }
            else
            {
                error("] missing (selector)");
            }
        }}
    }
}

void instruction()
{
    struct item_t * leftItem;
    struct item_t * rightItem;
    struct object_t * object;

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
        leftItem = malloc(sizeof(struct item_t));
        return_statement(leftItem);
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

            return;
        }

        if(tokenType == TOKEN_LRB) // procedure call
        {
            while(isIn(tokenType, FIRST_EXPRESSION))
            {
                expression(0);
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

        // while(tokenType == TOKEN_ACCESS)
        // {
        //     getNextToken();
        //     if(tokenType == TOKEN_IDENTIFIER)
        //     {
        //         // TODO: store identifier
        //         getNextToken();
        //     }
        //     else
        //     {
        //         error("Identifier expected (factor)");
        //         return;
        //     }
        // }

        // if(tokenType == TOKEN_LSB)
        // {
        //     getNextToken();
        //     if(isIn(tokenType, FIRST_EXPRESSION))
        //     {
        //         expression(0);
        //     }
        //     else
        //     {
        //         error("expression expected (factor)");
        //     }

        //     if(tokenType == TOKEN_RSB)
        //     {
        //         getNextToken();
        //     }
        //     else
        //     {
        //         mark("] expected (factor)");
        //         getNextToken();
        //     }
        // }

        leftItem = malloc(sizeof(struct item_t));

        // values still set from identifier before ASSIGNMENT
        object = findObject(objectGlobal); // implicitly uses stringValue
        if(object != 0)
        {
            leftItem->mode = CODEGEN_MODE_VAR;
            leftItem->type = object->type;

            // TODO: distinction between global and local scope
            leftItem->reg = CODEGEN_GP;

            leftItem->offset = object->offset;

            selector(leftItem); // array, record access
        }
        else
        {
            error("Undeclared variable");
        }

        if(tokenType == TOKEN_ASSIGNMENT) // assignment
        {
            getNextToken();

            rightItem = malloc(sizeof(struct item_t));
            expression(rightItem);

            assignmentOperator(leftItem, rightItem);

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
            expression(0);

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
            expression(0);

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
    struct item_t * item;

    if(tokenType == TOKEN_TYPEDEF)
    {
        getNextToken();

        if(isIn(tokenType, FIRST_TYPE))
        {
            item = malloc(sizeof(struct item_t));
            type(item);
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
                objectClass = CLASS_FIELD; 
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

        isGlobal = 1;
        objectClass = CLASS_VAR;
        addObjectToList();

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
    initCodeGen();

    errorCount = 0;
    warningCount = 0;
    tokenType = -1;
    openFile("test/m4.c");
    //openFile("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/test/m4.c");
    start();
    printf("Parsed with %d errors, %d warnings\n", errorCount, warningCount);

    return 0;
}
