#include <stdlib.h>
#include "scanner.c"

static int currentType;    //2 = int, 3 = char
static int isArray;
static int isStruct;
static int isGlobal; // 0 for local, 1 for global
static int objectClass;
static int lastOffsetPointer;
static int lastFieldPointer;

// -- Codegen
static int SIZE_INT;

static int CODEGEN_GP;
static int CODEGEN_MODE_CONST;
static int CODEGEN_MODE_VAR;
static int CODEGEN_MODE_REG;
static int CODEGEN_MODE_REF;
static int CODEGEN_MODE_COND;

static int PC;

static int FP;
static int SP;
static int LINK;
static int RR;

static int returnFJumpAddress;
static struct object_t * procedureContext;

static int * output;

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
    struct object_t * params;
    int value;
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
struct type_t *typeBool;


void initTypes(){
    typeInt = malloc(sizeof(struct type_t));
    typeChar = malloc(sizeof(struct type_t));
    typeBool = malloc(sizeof(struct type_t));
    
    typeInt->form = FORM_INT;
    typeInt->size = 4;
    typeChar->form = FORM_CHAR;
    typeChar->size = 4;
    typeBool->form = FORM_BOOL;
    typeBool->size = 4;
    lastOffsetPointer = 0;
    lastFieldPointer = 0;
    
}

void addToList()
{
    printf("Adding %s\n", stringValue);
}


struct object_t *findObject(struct object_t *firstElement, char *identifier){
    
    if(firstElement != 0){
        while (firstElement->next != 0) {
            if(strCompare(firstElement->name, identifier)){
                printf("\nSymbol table: found %s\n", stringValue);
                return firstElement;
            }
            firstElement = firstElement->next;
        }
        if(strCompare(firstElement->name, identifier)){
            printf("\nSymbol table: found %s\n", stringValue);
            return firstElement;
        }
    }
    
    return 0;
}

struct object_t *findProcedureObject(struct object_t *firstElement, char *identifier){
    
    if(firstElement != 0){
        while (firstElement->next != 0) {
            if(strCompare(firstElement->name, identifier) && firstElement->class == CLASS_PROC){
                printf("\nSymbol table: found %s\n", identifier);
                return firstElement;
            }
            firstElement = firstElement->next;
        }
        if(strCompare(firstElement->name, identifier) && firstElement->class == CLASS_PROC){
            printf("\nSymbol table: found %s\n", identifier);
            return firstElement;
        }
    }
    
    return 0;
}

struct object_t *createObject(){
    struct object_t *newObjectElement;
    struct object_t *newTempObject;
    
    newTempObject = malloc(sizeof(struct object_t));
    
    newObjectElement = malloc(sizeof(struct object_t));
    newObjectElement->name = malloc(sizeof(char) * 1024);
    
    if(isGlobal == 0){
        newTempObject = objectLocal;
    }
    
    if(isGlobal == 1){
        newTempObject = objectGlobal;
    }
    
    strCopy(stringValue, newObjectElement->name);
    newObjectElement->next = 0;
    
    if(newTempObject != 0){
        while (newTempObject->next != 0) {
            if(strCompare(newTempObject->name, stringValue)){
                return 0;
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

    return newObjectElement;
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
            tempTypeObject->offset = lastOffsetPointer - 4;
            lastOffsetPointer = lastOffsetPointer - 4;
            return 0;
        }
    }
    
    if(objectClass == 2 && (isArray == 0 && isStruct == 0)){
        if(currentType == FORM_INT){
            tempTypeObject->type = typeInt;
        }
        if(currentType == FORM_CHAR){
            tempTypeObject->type = typeChar;
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
            lastFieldElementGlobal->offset = lastFieldPointer;
            lastFieldPointer = lastFieldPointer + newType->size;
            lastObjectGlobal->type->size = lastObjectGlobal->type->size + 4;
        }
        return 0;
        
    }

    
    if(currentType == FORM_INT ){
        if(isGlobal == 0){
            lastFieldElementLocal->type = typeInt;
            lastObjectLocal->type->size = lastObjectLocal->type->size + 4;
        }
        if(isGlobal == 1){
            lastFieldElementGlobal->type = typeInt;
            lastFieldElementGlobal->offset = lastFieldPointer;
            lastFieldPointer = lastFieldPointer + typeInt->size;
            lastObjectGlobal->type->size = lastObjectGlobal->type->size + 4;
        }
    }
    
    if(currentType == FORM_CHAR){
        if(isGlobal == 0){
            lastFieldElementLocal->type = typeChar;
            lastObjectLocal->type->size = lastObjectLocal->type->size + 4;
        }
        if(isGlobal == 1){
            lastFieldElementGlobal->type = typeChar;
            lastFieldElementGlobal->offset = lastFieldPointer;
            lastFieldPointer = lastFieldPointer + typeChar->size;
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

void fail(char *message)
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

void error(char *message)
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

void mark(char *message)
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
    PC = 0;

    SIZE_INT = 4;

    RR = 27;
    FP = 29;
    SP = 30;
    LINK = 31;

    CODEGEN_MODE_CONST = 1;
    CODEGEN_MODE_VAR = 2;
    CODEGEN_MODE_REG = 3;
    CODEGEN_MODE_REF = 4;
    CODEGEN_MODE_COND = 5;

    output = malloc(1000 * sizeof(int));
}

struct item_t
{
    int mode; // CONST, VAR, REG, REF
    struct type_t * type;
    int reg; // both reg and offset give us the address of the variable
    int offset; // reg[reg] + offset -> address
    int value;

    int operator;
    int fls;
    int tru;
};

// TOKEN -> TARGET
int branch(int operator)
{
    if(operator == TOKEN_GREATER) { return TARGET_BGT; }
    if(operator == TOKEN_GREATEREQUAL) { return TARGET_BGE; }
    if(operator == TOKEN_UNEQUAL) { return TARGET_BNE; }
    if(operator == TOKEN_EQUAL) { return TARGET_BEQ; }
    if(operator == TOKEN_LESS) { return TARGET_BLE; }
    if(operator == TOKEN_LESSEQUAL) { return TARGET_BLT; }

    return 0;
}


// TOKEN -> TOKEN
int negate(int operator)
{
    if(operator == TOKEN_LESSEQUAL) { return TOKEN_GREATER; }
    if(operator == TOKEN_LESS) { return TOKEN_GREATEREQUAL; }
    if(operator == TOKEN_EQUAL) { return TOKEN_UNEQUAL; }
    if(operator == TOKEN_UNEQUAL) { return TOKEN_EQUAL; }
    if(operator == TOKEN_GREATER) { return TOKEN_LESSEQUAL; }
    if(operator == TOKEN_GREATEREQUAL) { return TOKEN_LESS; }

    return 0;
}

int requestRegister()
{
    int i;
    i = 1;
    while(isRegisterUsed[i] && (i <= 32))
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

int address(char *identifier)
{
    // TODO: Check if present in global symbol table

    // TODO: Sum of size of previous elements in symbol table.

    // TODO: Return size

    return -1;
} 

void storeString(struct item_t * item, char *string)
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

    printf("put %d %d %d %d\n", op, a, b, c);

    if (c < 0)
    {
        c = c + 65536; // 0x10000: 2^16
    }
    // if << is not available
    // replace (x << 5) by (x * 32) and (x << 16) by (x * 65536)
    instruction = (((((op * 32) + a) * 32) + b) * 65536) + c;

    output[PC] = instruction;

    PC = PC + 1;
}

void writeToFile(){
    int i;
    int instruction;

    struct object_t *tempTypeObject;
    
    tempTypeObject = malloc(sizeof(struct object_t));
    
    tempTypeObject = objectGlobal;
    
    if(tempTypeObject != 0){
        while(tempTypeObject->next != 0){
            if(tempTypeObject->class == CLASS_VAR){
                put(0, 0, 0, 0);
            }
            tempTypeObject = tempTypeObject->next;
        }
        if(tempTypeObject->class == CLASS_VAR){
            put(0, 0, 0, 0);
        }
    }

    i = 0;

    FILE * file = fopen("a.out", "w");
    while(i < PC)
    {
        instruction = output[i];
        
        fputc((instruction >> 24) & 255, file);
        fputc((instruction >> 16) & 255, file);
        fputc((instruction >> 8) & 255, file);
        fputc(instruction & 255, file);

        i = i + 1;
    }
    fclose(file);
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

void cJump(struct item_t * item)
{
    put(branch(negate(item->operator)), item->reg, 0, item->fls);
    releaseRegister(item->reg);
    item->fls = PC-1;
}

void bJump(int backAddress)
{
    put(TARGET_BR, 0, 0, backAddress-PC);
}

int fJump()
{
    put(TARGET_BR, 0, 0, 0);
    return PC-1;
}

void encodeC(int address, int c)
{
    // assumes c is 0
    output[address] = output[address] | c;
}

int decodeC(int address)
{
    return output[address] & 65535;
}

int concatenate(int listA, int listB)
{
    int previous;
    int current;

    if(listA == 0)
    {
        return listB;
    }

    if(listB == 0)
    {
        return listA;
    }

    current = listA;
    while(current != 0)
    {
        previous = current;
        current = decodeC(current);
    }

    encodeC(previous, listB);

    return listA;   
}

void fixUp(int branchAddress)
{
    encodeC(branchAddress, PC-branchAddress);
}

void fixLink(int branchAddress)
{
    int nextBranchAddress;

    while(branchAddress != 0)
    {
        nextBranchAddress = decodeC(branchAddress);
        fixUp(branchAddress);
        branchAddress = nextBranchAddress;
    }
}

void unloadBool(struct item_t * item)
{
    if(item->mode == CODEGEN_MODE_COND)
    {
        cJump(item);
        fixLink(item->tru);
        item->mode = CODEGEN_MODE_REG;

        put(TARGET_ADDI, item->reg, 0, 1); // true
        put(TARGET_BR, 0, 0, 2);

        fixLink(item->fls);

        put(TARGET_ADDI, item->reg, 0, 0); // false
    }
}

void loadBool(struct item_t * item)
{
    if(item->mode != CODEGEN_MODE_COND)
    {
        load(item);

        item->mode = CODEGEN_MODE_COND;
        item->operator = TOKEN_UNEQUAL;
        item->fls = 0;
        item->tru = 0 ;
    }
}

void factorOperator(struct item_t * item)
{
    int tmp;

    if(item->type == typeBool)
    {
        loadBool(item);

        tmp = item->fls;
        item->fls = item->tru;
        item->tru = tmp;

        item->operator = negate(item->operator);
    }
    else
    {
        error("Boolean expression expected");
    }
}

void assignmentOperator(
    struct item_t * leftItem,
    struct item_t * rightItem)
{
    if(leftItem->type != rightItem->type)
    {
        mark("type mismatch in assignment");
    }

    if(rightItem->type == typeBool)
    {
        unloadBool(rightItem);
    }
    load(rightItem);

    // leftItem must be in VAR_MODE or REF_MODE
    // rightItem must be in REG_MODE
    put(TARGET_SW, rightItem->reg, leftItem->reg, leftItem->offset);

    if(leftItem->mode == CODEGEN_MODE_REF)
    {
        releaseRegister(leftItem->reg);
    }

    releaseRegister(rightItem->reg);
    
}

void termAND(struct item_t * item)
{
    if(item->type == typeBool)
    {
        loadBool(item);

        put(branch(negate(item->operator)), item->reg, 0, item->fls);
        releaseRegister(item->reg);
        item->fls = PC-1;
        fixLink(item->tru);
        item->tru = 0;
    }
    else
    {
        error("Boolean expression expected in &&");
    }
}

void termBinaryOperator(
    struct item_t * leftItem,
    struct item_t * rightItem,
    int operatorSymbol)
{
    if(operatorSymbol == TOKEN_AND)
    {
        if((leftItem->type == typeBool) && (rightItem->type == typeBool))
        {
            loadBool(rightItem);

            leftItem->reg = rightItem->reg;
            leftItem->fls = concatenate(rightItem->fls, leftItem->fls);
            leftItem->tru = rightItem->tru;
            leftItem->operator = rightItem->operator;
        }
        else
        {
            error("Boolean expressions expected (&&)");
        }
    }

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

void expressionOperator(
    struct item_t * leftItem,
    struct item_t * rightItem,
    int operatorSymbol)
{
    if((leftItem->type == typeInt) && (rightItem->type == typeInt))
    {
        load(leftItem);
        if((rightItem->mode != CODEGEN_MODE_CONST) || (rightItem->value != 0 ))
        {
            load(rightItem);
            
            put(TARGET_CMP, leftItem->reg, leftItem->reg, rightItem->reg);
            releaseRegister(rightItem->reg);
        }
        leftItem->mode = CODEGEN_MODE_COND;
        leftItem->type = typeBool;
        leftItem->operator = operatorSymbol;
        leftItem->fls = 0;
        leftItem->tru = 0;
    }
    else
    {
        error("Integer expressions expected");
    }
}

void simpleExpressionBinaryOperator(
    struct item_t * leftItem,
    struct item_t * rightItem,
    int operatorSymbol)
{
    if(operatorSymbol == TOKEN_OR)
    {
        if((leftItem->type == typeBool) && (rightItem->type == typeBool))
        {
            loadBool(rightItem);

            leftItem->reg = rightItem->reg;
            leftItem->fls = rightItem->fls;
            //leftItem->tru = concatenate(rightItem->tru, leftItem->tru);
            leftItem->operator = rightItem->operator;
        }
        else
        {
            error("Boolean expressions exptected");
        }
        return;
    }

    if((leftItem->type == typeInt) && (rightItem->type == typeInt))
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
                else{
                    if(operatorSymbol == TOKEN_MINUS){
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

void simpleExpressionOR(struct item_t * item)
{
    if(item->type == typeBool)
    {
        loadBool(item);

        put(branch(item->operator), item->reg, 0, item->tru);
        releaseRegister(item->reg);
        item->tru = PC-1;
        fixLink(item->fls);
        item->fls = 0;
    }
    else
    {
        error("Boolean expression expected in ||");
    }
}

// -----------------------------------------------------------------------------

int isIn(int tokenType, int rule) {

    if(rule == FIRST_TOP_DECLARATION && (
        (tokenType == TOKEN_TYPEDEF) ||
        (tokenType == TOKEN_STRUCT) ||
        (tokenType == TOKEN_STATIC) ||
        isIn(tokenType, FIRST_TYPE) ))
    { return 1; }

    if(rule == FIRST_TYPE_DECLARATION && (
        (tokenType == TOKEN_STRUCT) ||
        (tokenType == TOKEN_TYPEDEF)))
    { return 1; }

    if(rule == FIRST_BASIC_TYPES && (
        (tokenType == TOKEN_INT) ||
        (tokenType == TOKEN_CHAR)))
    { return 1; }

    if(rule == FIRST_EXPRESSION && (
        (tokenType == TOKEN_MINUS) ||
        (tokenType == TOKEN_IDENTIFIER) ||
        (tokenType == TOKEN_CONSTINT) ||
        (tokenType == TOKEN_CONSTCHAR) ||
        (tokenType == TOKEN_STRING_LITERAL) ||
        (tokenType == TOKEN_LRB) ||
        (tokenType == TOKEN_FCLOSE) ||
        (tokenType == TOKEN_FOPEN) ||
        (tokenType == TOKEN_SIZEOF) ||
        (tokenType == TOKEN_MALLOC)))
    { return 1; }

    if(rule == FIRST_TYPE && (
        (tokenType == TOKEN_INT) ||
        (tokenType == TOKEN_CHAR) ||
        (tokenType == TOKEN_VOID) ||
        (tokenType == TOKEN_STRUCT )))
    { return 1; }

    if(rule == FIRST_VARIABLE_DECLARATION && (
        (tokenType == TOKEN_STATIC) ||
        isIn(tokenType, FIRST_TYPE) ))
    { return 1; }

    if(rule == FIRST_INSTRUCTION && (
        isIn(tokenType, FIRST_VARIABLE_DECLARATION) ||
        isIn(tokenType, FIRST_TYPE_DECLARATION) ||
        (tokenType == TOKEN_IDENTIFIER) ||
        (tokenType == TOKEN_WHILE) ||
        (tokenType == TOKEN_IF)))
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
void procedureCall(struct item_t * item);

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
        item->type = typeChar;
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
        object = findObject(objectGlobal,stringValue);

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

            object = findObject(objectGlobal,stringValue);

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
                    //if(item->mode == CODEGEN_MODE_CONST) // only allow allocation known during compile time
                    //{
                        load(item);
                        put(TARGET_MALLOC, item->reg, 0, item->reg);
                    // }
                    // else
                    // {
                    //     error("Invalid allocation");
                    // }
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

    if(tokenType == TOKEN_NOT) {
        getNextToken();

        factor(item);
        factorOperator(item);

        return;
    }

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

        object = findObject(objectLocal,stringValue); // implicitly uses stringValue
        if(object == 0){
            findObject(objectGlobal, stringValue);
        }
        getNextToken();

        if(tokenType == TOKEN_LRB)
        {
            procedureCall(item);

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

    while((tokenType == TOKEN_MULT) || (tokenType == TOKEN_DIVIDE) || (tokenType == TOKEN_PERCENT) || (tokenType == TOKEN_AND))
    {
        operatorSymbol = tokenType;
        if(operatorSymbol == TOKEN_AND)
        {
            termAND(item);
        }
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

    while((tokenType == TOKEN_PLUS) || (tokenType == TOKEN_MINUS) || (tokenType == TOKEN_OR))
    {
        operatorSymbol = tokenType;
        if(operatorSymbol == TOKEN_OR)
        {
            simpleExpressionOR(item);
        }
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        term(rightItem);
        simpleExpressionBinaryOperator(item, rightItem, operatorSymbol);
    }
}

void expression(struct item_t * item)
{
    struct item_t * rightItem;

    simple_expression(item);
    
    if(tokenType == TOKEN_EQUAL)
    {
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        expression(rightItem);
        expressionOperator(item, rightItem, TOKEN_EQUAL);
        return;
    }
    if(tokenType == TOKEN_LESSEQUAL)
    {
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        expression(rightItem);
        expressionOperator(item, rightItem, TOKEN_LESSEQUAL);
        return;
    }
    if(tokenType == TOKEN_LESS)
    {
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        expression(rightItem);
        expressionOperator(item, rightItem, TOKEN_LESS);
        return;
    }
    if(tokenType == TOKEN_UNEQUAL)
    {
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        expression(rightItem);
        expressionOperator(item, rightItem, TOKEN_UNEQUAL);
        return;
    }
    if(tokenType == TOKEN_GREATER)
    {
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        expression(rightItem);
        expressionOperator(item, rightItem, TOKEN_GREATER);
        return;
    }
    if(tokenType == TOKEN_GREATEREQUAL)
    {
        getNextToken();
        rightItem = malloc(sizeof(struct item_t));
        expression(rightItem);
        expressionOperator(item, rightItem, TOKEN_GREATEREQUAL);
        return;
    }
    // if(tokenType == TOKEN_AND)
    // {
    //     getNextToken();
    //     rightItem = malloc(sizeof(struct item_t));
    //     expression(rightItem);
    //     expressionOperator(item, rightItem, TOKEN_LESS);
    //     return;
    // }
    // if(tokenType == TOKEN_OR)
    // {
    //     getNextToken();
    //     simpleExpressionOR(item);
    //     rightItem = malloc(sizeof(struct item_t));
    //     expression(rightItem);
    //     expressionOperator(item, rightItem, TOKEN_LESS);
    //     return;
    // }
    // if(tokenType == TOKEN_ASSIGNMENT)
    // {
    //     getNextToken();
    //     expression(0);
    //     return;
    // }
}

void pushParameter(struct item_t * item)
{
    if(item->type == typeBool)
    {
        unloadBool(item);
    }

    load(item);

    put(TARGET_PSH, item->reg, SP, 4);

    releaseRegister(item->reg);
}

struct object_t * createAnonymousParameter(struct object_t * object, struct type_t * type)
{
    return 0;
}

struct object_t * actualParameter(
    struct object_t * object, 
    struct object_t * formalParameter)
{
    struct item_t * item;

    if(isIn(tokenType, FIRST_EXPRESSION))
    {
        item = malloc(sizeof(struct item_t));
        expression(item);

        if(formalParameter != 0)
        {
            if(item->type != formalParameter->type)
            {
                mark("Type mismatch in procedure call");
            }
        }
        else
        {
            formalParameter = createAnonymousParameter(object, item->type);
        }

        pushParameter(item);

        formalParameter = formalParameter->next;
    }
    else
    {
        error("Actual parameter expected");
    }

    return formalParameter;
}

void actualParameters(struct object_t * object)
{
    struct object_t * nextFormalParameter;
    struct item_t * item;

    if(tokenType == TOKEN_LRB)
    {
        nextFormalParameter = object->params;
        if(isIn(tokenType, FIRST_EXPRESSION))
        {
            nextFormalParameter = actualParameter(object, nextFormalParameter);
            while(tokenType == TOKEN_COMMA)
            {
                getNextToken();
                nextFormalParameter = actualParameter(object, nextFormalParameter);
            }
        }

        while(nextFormalParameter != 0)
        {
            mark("Actual parameter expected");

            item = malloc(sizeof(struct item_t));

            item->mode = CODEGEN_MODE_CONST;
            item->type = typeInt;
            item->value = 0;

            pushParameter(item);

            nextFormalParameter = nextFormalParameter->next;
        }

        if(tokenType == TOKEN_RRB)
        {
            getNextToken();
        }
        else
        {
            mark(") expected (actualParameters)");
            getNextToken();
        }

    }
}

int sJump(int branchAddress)
{
    put(TARGET_BSR, 0, 0, branchAddress);
    return PC - 1;
}

int isBR(int address)
{
    return 0;
}

void pushUsedRegisters()
{

}

void popUsedRegisters()
{

}

void procedureCall(struct item_t * item)
{
    struct object_t * object;

    object = findObject(objectGlobal,stringValue);
    if(object == 0)
    {
        mark("undeclared procedure procedureCall");
        isGlobal = 1;
        object = createObject();

        object->class = CLASS_PROC;
        object->type = 0; // = UNKNOWN_TYPE; // TODO
        object->offset = 0;
    }

    item->mode = CODEGEN_MODE_REG;
    item->type = object->type;
    pushUsedRegisters();
    actualParameters(object);
    if((object->offset != 0) && ! isBR(object->offset))
    {  
        sJump(object->offset - PC);
    }
    else
    {
        object->offset = sJump(object->offset);
    }

    popUsedRegisters();
    item->reg = requestRegister();
    put(TARGET_ADD, item->reg, 0, RR);
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

int fJumpChain(int branchAddress)
{
    put(TARGET_BR, 0, 0, branchAddress);
    return PC - 1;
}

void return_statement(struct item_t * item)
{
    if(tokenType == TOKEN_RETURN)
    {
        getNextToken();

        if(isIn(tokenType, FIRST_EXPRESSION))
        {
            expression(item);
            if(item->type != procedureContext->type)
            {
                mark("return type mismatch (return_statement)");
            }

            if(item->type == typeBool)
            {
                unloadBool(item);
            }
            load(item);
            put(TARGET_ADD, RR, 0, item->reg);
            releaseRegister(item->reg);
        }

        returnFJumpAddress = fJumpChain(returnFJumpAddress);
    }
    else
    {
        error("Return expected (return_statement)");
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
                object = findObject(item->type->fields,stringValue);
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

        leftItem = malloc(sizeof(struct item_t));

        // values still set from identifier before ASSIGNMENT
        object = findObject(objectLocal,stringValue); // implicitly uses stringValue
        if(object == 0){
            object = findObject(objectGlobal, stringValue);
        }
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

        
        objectClass = CLASS_VAR; 
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
    struct item_t * item;
    int fJumpAddress;

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
            item = malloc(sizeof(struct item_t));
            expression(item);

            if(item->type == typeBool)
            {
                loadBool(item);
                cJump(item);
                fixLink(item->tru);
            }
            else
            {
                error("boolean expression expected (if_else)");
            }

            if(tokenType == TOKEN_RRB)
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
                getNextToken();
                fJumpAddress = fJump();
                fixLink(item->fls);

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
                fixUp(fJumpAddress);
            }
            else
            {
                fixLink(item->fls);
            }
        }
    }
}

void while_loop()
{
    struct item_t * item;
    int bJumpAddress;

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
            bJumpAddress = PC;

            item = malloc(sizeof(struct item_t));
            expression(item);

            if(item->type == typeBool)
            {
                loadBool(item);
                cJump(item);
                fixLink(item->tru);
            }
            else
            {
                error("Boolean expression expected (while_loop)");
            }
            if(tokenType == TOKEN_RRB)
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

            bJump(bJumpAddress);
            fixLink(item->fls);
        }
        else
        {
            error("expression expected (while_loop)");
        }
    }
}

struct type_t * basicArrayRecordType()
{

    struct type_t * newType;
    struct object_t * newObject;
    struct item_t *item;
    

    if(isIn(tokenType, FIRST_TYPE))
    {
        newType = malloc(sizeof(struct type_t));
        newObject = malloc(sizeof(struct object_t));
        item = malloc(sizeof(struct item_t));
        
        type(item);
        
        if(currentType == TOKEN_INT){
            if(tokenType == TOKEN_MULT){
                newType->form = FORM_ARRAY;
                newType->base = typeInt;
                getNextToken();
                return newType;
            }
            
            return typeInt;
            
        }
        
        if(currentType == TOKEN_CHAR){
            if(tokenType == TOKEN_MULT){
                newType->form = FORM_ARRAY;
                newType->base = typeChar;
                getNextToken();
                return newType;
            }
            return typeChar;
            
        }
        
        if(currentType == TOKEN_STRUCT){

            if(findObject(objectLocal, stringValue) != 0){
                newObject = findObject(objectLocal, stringValue);
                getNextToken();
                return newObject->type;
            }
            
            if(findObject(objectGlobal, stringValue) != 0){
                newObject = findObject(objectGlobal, stringValue);
                getNextToken();
                return newObject->type;
            }
            
        }
        

    }
    else
    {
        error("Unknown type");
        return 0;
    }
    
    return 0;
    
}

struct  object_t * createFormalParameter(
    struct object_t * object,
    struct type_t * type,
    char * identifier)
{
    struct object_t * newObject;

    newObject = object->params;

    if(object->params != 0)
    {
        while(newObject->next != 0)
        {
            newObject = newObject->next;
        }

        newObject->next = malloc(sizeof(struct object_t));
        newObject->next->name = malloc(sizeof(char) * 1024);
        strCopy(identifier, newObject->next->name);
        newObject->next->type = type;

        return newObject->next;
    }
    else
    {
        object->params = malloc(sizeof(struct object_t));
        object->params->name = malloc(sizeof(char) * 1024);
        strCopy(identifier, object->params->name);
        object->params->type = type;

        return object->params;
    }
}

struct object_t * formalParameter(
    struct object_t * object,
    struct object_t * formalParameter)
{
    struct type_t * type;

    type = basicArrayRecordType();

    if(tokenType == TOKEN_IDENTIFIER)
    {
        if(formalParameter != 0)
        {
            if(type != formalParameter->type)
            {
                mark("type mismatch in procedure declaration and call (formalParameter)");
            }

            if(findObject(object->params,stringValue) != 0)
            {
                error("Parameter name already used (formalParameter)");
            }
            formalParameter->name = stringValue;

        }
        else
        {
            formalParameter = createFormalParameter(object, type, stringValue);
        }
        getNextToken();
        formalParameter = formalParameter->next;
    }
    else
    {
        error("Expected identifier (formalParameter)");
    }
    return formalParameter;
}

void formalParameters(struct object_t * object)
{
    int numberOfParameters;
    struct object_t * nextParameter;

    numberOfParameters = 0;

    if(tokenType == TOKEN_LRB)
    {
        getNextToken();
    }
    else
    {
        mark("( expected (formalParameters)");
        getNextToken();
    }

    nextParameter = object->params;

    if(isIn(tokenType, FIRST_VARIABLE_DECLARATION))
    {
        nextParameter = formalParameter(object, nextParameter);
        numberOfParameters = numberOfParameters + 1;

        while(tokenType == TOKEN_COMMA)
        {
            getNextToken();
            nextParameter = formalParameter(object, nextParameter);
            numberOfParameters = numberOfParameters + 1;
        }
    }

    object->value = numberOfParameters;
    nextParameter = object->params;

    while(nextParameter != 0)
    {
        numberOfParameters = numberOfParameters - 1;
        nextParameter->offset = numberOfParameters * 4 + 8;
        nextParameter = nextParameter->next;
    }

    if(tokenType == TOKEN_RRB)
    {
        getNextToken();
    }
    else
    {
        mark(") expected (formalParameters)");
        getNextToken();
    }
}

void prologue(int localSize)
{
    put(TARGET_PSH, LINK, SP, 4);
    put(TARGET_PSH, FP, SP, 4);
    put(TARGET_ADD, FP, 0, SP);
    put(TARGET_SUBI, SP, SP, localSize);
}

void epilogue(int paramSize)
{
    put(TARGET_ADD, SP, 0, FP);
    put(TARGET_POP, FP, SP, 4);
    put(TARGET_POP, LINK, SP, paramSize + 4);
    put(TARGET_RET, 0, 0, LINK);
}

int variableDeclarationSequence(struct object_t * object)
{
    struct object_t * currentObject;
    int x;

    x = 0;
    currentObject = object->params;

    while(currentObject != 0)
    {
        x = x + 1;
        currentObject = currentObject->next;
    }

    return x;
}

void function_declaration()
{
    struct item_t * item;
    struct object_t * object;

    if(isIn(tokenType, FIRST_TYPE))
    {
        item = malloc(sizeof(struct item_t));
        type(item);

        if(tokenType == TOKEN_IDENTIFIER)
        {
            object = findProcedureObject(objectGlobal, stringValue);
            if(object != 0) // the procedure appeared before
            {
                if(object->type != item->type)
                {
                    mark("return type mismatch in procedure");
                }
                fixLink(object->offset);
            }
            else // the procedure is newly declared
            {
                isGlobal = 1;
                object = createObject();
                object->class = CLASS_PROC;

                getNextToken();

                object->type = item->type;
                object->offset = PC;

                formalParameters(object); // TODO: set object->value to # of param
            }
            
            if(tokenType == TOKEN_SEMICOLON)
            {
                getNextToken();
                return;
            }

            returnFJumpAddress = 0;

            if(tokenType == TOKEN_LCB)
            {
                getNextToken();
            }
            else
            {
                mark("{ expected (function_declaration)");
                getNextToken();
            }

            prologue(variableDeclarationSequence(object) * 4);
            procedureContext = object;

            while(isIn(tokenType, FIRST_INSTRUCTION))
            {
                instruction();
            }

            fixLink(returnFJumpAddress);
            epilogue(object->value * 4);

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

int main(int argc, char ** argv){
    printf("\nPhoenix: Parser\n===============\n");
    initTokens();

    initTypes();
    initCodeGen();
    errorCount = 0;
    warningCount = 0;
    tokenType = -1;
    if(argc == 1)
    {
        openFile("test/m6.c");
    }
    else
    {
        openFile(argv[1]);
    }
    start();
    writeToFile();
    printf("Parsed with %d errors, %d warnings\n", errorCount, warningCount);

    return 0;
}
