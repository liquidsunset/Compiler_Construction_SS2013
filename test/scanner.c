
#include <stdio.h> // used for tests

static int FIRST_FUNCTION_DEFINITION;
static int FIRST_GLOBAL_VARIABLE_DECLARATION;
static int FIRST_TYPE;
static int FIRST_VARIABLE_DECLARATION;
static int FIRST_EXPRESSION;
static int FIRST_FUNCTION_STATEMENT;
static int FIRST_TYPE_DECLARATION;
static int FIRST_INSTRUCTION;
static int FIRST_TOP_DECLARATION;
static int FIRST_BASIC_TYPES;
//tokens:

//Keywords
static int TOKEN_NULL;
static int TOKEN_VOID;
static int TOKEN_INT;
static int TOKEN_CHAR;
static int TOKEN_DOUBLE;
static int TOKEN_WHILE;
static int TOKEN_IF;
static int TOKEN_ELSE;
static int TOKEN_RETURN;
static int TOKEN_STRUCT;
static int TOKEN_STATIC;
static int TOKEN_FOPEN;
static int TOKEN_FCLOSE;
static int TOKEN_FGETC;
static int TOKEN_FPUTC;
static int TOKEN_MALLOC;
static int TOKEN_SIZEOF;
static int TOKEN_TYPEDEF;
static int TOKEN_PRINTF;

//Identififier
static int TOKEN_IDENTIFIER;

//Konstanten
static int TOKEN_CONSTINT;
static int TOKEN_CONSTDOUBLE;
static int TOKEN_CONSTCHAR;

//String-Literale
static int TOKEN_STRING_LITERAL;

//Operatoren
static int TOKEN_PLUS;
static int TOKEN_MINUS;
static int TOKEN_MULT;
static int TOKEN_DIVIDE;
static int TOKEN_PERCENT;
static int TOKEN_ASSIGNMENT;
static int TOKEN_EQUAL;
static int TOKEN_LESSEQUAL;
static int TOKEN_GREATEREQUAL;
static int TOKEN_LESS;
static int TOKEN_GREATER;
static int TOKEN_SHIFTLEFT;
static int TOKEN_SHIFTRIGHT;
static int TOKEN_ADDRESS;
static int TOKEN_AND;
static int TOKEN_BITWISEOR;
static int TOKEN_OR;
static int TOKEN_BITWISEEXCLOR;
static int TOKEN_BITWISENOT;
static int TOKEN_UNEQUAL;
static int TOKEN_INCREMENT;
static int TOKEN_DECREMENT;
static int TOKEN_NOT;
static int TOKEN_ACCESS;

//Punktuatoren

static int TOKEN_LSB;
static int TOKEN_RSB;
static int TOKEN_LRB;
static int TOKEN_RRB;
static int TOKEN_LCB;
static int TOKEN_RCB;
static int TOKEN_SEMICOLON;
static int TOKEN_COMMA;
static int TOKEN_COLON;
static int TOKEN_EOF;
static int TOKEN_INCLUDE;
static int TOKEN_POINT;

static int CLASS_VAR;
static int CLASS_TYPE;
static int CLASS_FIELD;
static int CLASS_PROC;
static int CLASS_STRING;

static int FORM_ARRAY;
static int FORM_RECORD;
static int FORM_INT;
static int FORM_CHAR;
static int FORM_BOOL;

static int errorCount;
static int warningCount;

// String literals:
static int isInString;
static int isInChar;

static int currentChar;
static int nextChar;


//Scanner:
static char *stringValue;

//Parser:
static char *typeName; //name from struct or array
//Parser_Codegen:
static int *isRegisterUsed;

// ------ Margit -----------
static int TARGET_NOP;
static int TARGET_ADD;
static int TARGET_ADDI;
static int TARGET_AND;
static int TARGET_DIV;
static int TARGET_DIVI;
static int TARGET_J;
static int TARGET_LW;
static int TARGET_MUL;
static int TARGET_MULI;
static int TARGET_OR;
static int TARGET_SUB;
static int TARGET_SUBI;
static int TARGET_SW;
static int TARGET_MODI;
static int TARGET_CMPI;
static int TARGET_MOD;
static int TARGET_CMP;
static int TARGET_POP;
static int TARGET_PSH;
static int TARGET_BEQ;
static int TARGET_BGE;
static int TARGET_BGT;
static int TARGET_BLE;
static int TARGET_BLT;
static int TARGET_BNE;
static int TARGET_BR;
static int TARGET_BSR;
static int TARGET_RET;
static int TARGET_JSR;
static int TARGET_FLO;
static int TARGET_FLC;
static int TARGET_RDC;
static int TARGET_WRC;
static int TARGET_MALLOC;
static int TARGET_TRAP;
static int TARGET_PRINTF;
static int TARGET_PRINTFI;



static int fp;

static int tokenType;
static int intValue;
static int positionLine;
static int positionColumn;
static int eofFlag;

static int lin;
static int col;


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
    TOKEN_FOPEN = 11;
    TOKEN_FCLOSE = 12;
    TOKEN_FGETC = 13;
    TOKEN_FPUTC = 14;
    TOKEN_MALLOC = 15;
    TOKEN_SIZEOF = 16;
    TOKEN_TYPEDEF = 17;
    TOKEN_PRINTF = 18;
    
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
    TOKEN_ACCESS = 423;
    
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
    FIRST_FUNCTION_STATEMENT = 5;
    FIRST_TYPE_DECLARATION = 6;
    FIRST_INSTRUCTION = 7;
    FIRST_TOP_DECLARATION = 8;
    FIRST_BASIC_TYPES = 9;
    
    CLASS_FIELD = 0;
    CLASS_TYPE = 1;
    CLASS_VAR = 2;
    CLASS_PROC = 3;
    CLASS_STRING = 4;
    
    FORM_RECORD = 0;
    FORM_ARRAY = 1;
    FORM_INT = 2;
    FORM_CHAR = 3;
    FORM_BOOL = 4;
    
    errorCount = 0;
    warningCount = 0;
    
    isInString = 0;
    isInChar = 0;
    
    currentChar = -1;
    nextChar = -1;
    
    // ---------- Margit ----
    
    TARGET_NOP = 0;
    
    // F1 (1-23)
    TARGET_ADDI = 1;
    TARGET_SUBI = 2;
    TARGET_MULI = 3;
    TARGET_DIVI = 4;
    TARGET_MODI = 5;
    TARGET_CMPI = 6;
    TARGET_LW = 7;
    TARGET_SW = 8;
    TARGET_POP = 9;
    TARGET_PSH = 10;
    TARGET_BEQ = 11;
    TARGET_BGE = 12;
    TARGET_BGT = 13;
    TARGET_BLE = 14;
    TARGET_BLT = 15;
    TARGET_BNE = 16;
    TARGET_BR = 17;
    TARGET_BSR = 18;
    TARGET_MALLOC = 19;
    TARGET_RET = 20;
    
    // F2 (23-43)
    TARGET_ADD = 23;
    TARGET_SUB = 24;
    TARGET_MUL = 25;
    TARGET_DIV = 26;
    TARGET_MOD = 27;
    TARGET_CMP = 28;
    TARGET_RET = 29;
    TARGET_FLO = 30;
    TARGET_FLC = 31;
    TARGET_RDC = 32;
    TARGET_WRC = 33;
    TARGET_AND = 34;
    TARGET_OR = 35;
    TARGET_PRINTF = 36;
    TARGET_PRINTFI = 37;
    
    // F3 (43-63)
    TARGET_JSR = 43;
    TARGET_J = 44;
    TARGET_TRAP = 45;
    
    stringValue = malloc(sizeof(char) * 1024);
    typeName = malloc(sizeof(char) * 1024);
    isRegisterUsed = malloc(sizeof(int) *32);
}





// ---------------------------- Tools -----------------------------------------

// Compares two character arrays.
// 
// Returns true if both arrays hold the same characters.
int strCompare(char *a, char *b)
{
	int i;
    i = 0;

	while( (i < 1024) && ((a[i] != 0) || (b[i] != 0))) {
		if( a[i] != b[i] )
		{
			return 0;
		}

		i = i + 1;
        
	}

	return 1;
}

// Length of string
//
// Returns the length of the string or 1024 if the string is unterminated
int strLength(char *a)
{
	int i;
    i = 0;
	while((a[i]!=0) && ( i<1024 )){
        i=i+1;    
    }
    
	return i;
}

// String copy
//
// Copies the first char array into the second
void strCopy(char *from, char *to)
{
	int i;
    i = 0;
	while( (from[i] != 0) && (i < 1023))
	{
		to[i]=from[i];
		i = i+1;
	}
	to[i]=0;
}

void strTrimQuotes(char *a, char *b)
{
    int i;
    i = 0;
    
    b[i] = a[i+1];
    i = i +1;
    
    while((a[i] != 39) && (a[i] != 34) && (a[i] != 0) && (i < 1024)){
        b[i] = a[i+1];
        i = i +1;
    }
    
    b[i-1] = 0;
}

// Checks if the character c is a letter.
// Umlauts are not supported, our input encoding is ASCII.
//
// Returns true if the character is within the range of [a-zA-Z]
int isLetter(char c)
{
	if(((c >= 97) && (c <= 122)) || ((c >= 65) && (c <= 90)) || (c == 95))
    {
        return 1;
    }
    
    return 0;
}

// Checks if the character c is a digit.
//
// Returns true if the character is within the range of [0-9]
int isDigit(char c)
{
	if((c >= 48) && (c <= 57))
    {
        return 1;
    }
    return 0;
}

// Checks if the character c is a whitespace:
// Space, Tabs, CR, LF.
//
// Returns true if the character is whitespace.
int isWhitespace(char c)
{
	if((c == 32) || (c == 10) || (c == 13) || (c == 9))
    {
        return 1;
    }

    return 0;
}

int isTerminalChar(char c)
{
	if(
        (c == 40 ) ||
        (c == 41 ) ||
        (c == 91 ) ||
        (c == 93 ) ||
        (c == 123 ) ||
        (c == 125 ) ||
        (c == 59 ) ||
        (c == 44 ) ||
        (c == 39) ||
        (c == 34)
        )
    {
        return 1;
    }

    return 0;
}

int isOperator(char c)
{
	if(!(isWhitespace(c)>0) && !(isLetter(c)>0) && !(isDigit(c)>0))
    {
        return 1;
    }
    return 0;
}

int characterClass(char c)
{
	if(isDigit(c) > 0) {return 1;}
	if(isLetter(c) > 0) {return 2;}

	return 3; // Terminal character
}
// ----------------------------------------------------------------------------

// ------------------------ Lexical analysis ----------------------------------

// Returns:
// 	0 if current token continues
// 	1 if it is terminated
int peek(int current, int next)
{
    char ccurrent;
    char cnext;

    ccurrent = current;
    cnext = next;

    if((current == 34) && !(isInChar>0) && !(isInString>0)) // add the starting "
    {
        isInString = 1;
        return 0;
    }
    if((next == 34) && (isInString>0)) {return 0;} // add the ending "
    if((current == 34) && (isInString>0)) // terminate after the ending "
    {
        isInString = 0;
        return 1;
    }
    if(isInString>0) {return 0;}

    // Char literals (duplicated to support something like "'a'"):

    if((current == 39) && !(isInChar>0)) // add the starting "
    {
        isInChar = 1;
        return 0;
    }
    if((next == 39) && (isInChar>0)) {return 0;} // add the ending "
    if((current == 39) && (isInChar>0)) // terminate after the ending "
    {
        isInChar = 0;
        return 1;
    }
    if(isInChar>0) {return 0;}

    if((current < 0) || (next < 0)) {return 1;} // EOF
	if(isWhitespace(cnext) > 0) {return 1;} // Whitespace always terminates.
	if((isTerminalChar(ccurrent)>0) || (isTerminalChar(cnext)>0)){ return 1;} // Brackets always terminate (as they are single char tokens)

    //if(current == 39 || next == 39) return 0; // char literals

    //if(current == 34 && isInString)
    //{
    //    isInString = 1;
    //    return 0;
    //}
    if(current == 35) {return 0;}

    if((isLetter(ccurrent)>0) && (isLetter(cnext)>0)) {return 0;} //Letter letter: avg
    if((isDigit(ccurrent)>0) && (isDigit(cnext)>0)) {return 0;} // Digit digit: 42
    if((isLetter(ccurrent)>0) && (isDigit(cnext)>0)) {return 0;} // Letter digit: List1

    if ((isOperator(ccurrent)>0) && (isOperator(cnext)>0)) {return 0;}

	return 1;
}

int power(int base, int exp)
{
    int i;
    int res;

    i = 0;
    res = 1;
    while(i < exp)
    {
        res = res * base;
        i = i + 1;
    }
    return res;
}

int strToInt(char *str)
{
    int i;
    int len;
    int res;
    int temp;

    i = 0;
    len = strLength(str);
    res = 0;

    if(str[0] == 45)
    {
        i = i + 1;
    }

    while((str[i] != 0) && (i < 1024))
    {
        if((str[i] >= 48) && (str[i] <= 57))
        {
            temp = str[i];
            res = res + (temp-48) * power(10, len-i-1);
        }
        i = i + 1;
    }

    if(str[0] == 45)
    {
        res = -res;
    }

    return res;
}
// ----------------------------------------------------------------------------

// creates the Filepointer
void openFile(char *path){

    lin = 1;
    col = 1;
    fp = fopen(path,"r");
}

//Returns the next Character from the file
int readNextCharacter(){
    int temp;

    if(fp == 0){
        return -1;
    }

    
    temp = fgetc(fp);

    if(temp < 0)
    {
        fclose(fp);
        return -1;
    }
    col = col + 1;
    if(temp == 10)
    {
        col = 1;
        lin = lin + 1;
        //printf("\n");
    }

    return temp;

}



void findToken(char *status,int len){
    
    char tokenChar;
    
    if(len == 1)
    {
        tokenChar = status[0];
        if(tokenChar == 43){tokenType = TOKEN_PLUS; return;}
        if(tokenChar == 45){tokenType = TOKEN_MINUS; return;}
        if(tokenChar == 42){tokenType = TOKEN_MULT; return;}
        if(tokenChar == 47){tokenType = TOKEN_DIVIDE; return;}
        if(tokenChar == 37){tokenType = TOKEN_PERCENT; return;}
        if(tokenChar == 61){tokenType = TOKEN_ASSIGNMENT; return;}
        if(tokenChar == 60){tokenType = TOKEN_LESS; return;}
        if(tokenChar == 62){tokenType = TOKEN_GREATER; return;}
        if(tokenChar == 38){tokenType = TOKEN_ADDRESS; return;}
        if(tokenChar == 124){tokenType = TOKEN_BITWISEOR; return;}
        if(tokenChar == 94){tokenType = TOKEN_BITWISEEXCLOR; return;}
        if(tokenChar == 126){tokenType = TOKEN_BITWISENOT; return;}
        if(tokenChar == 33){tokenType = TOKEN_NOT; return;}
        if(tokenChar == 91){tokenType = TOKEN_LSB; return;}
        if(tokenChar == 93){tokenType = TOKEN_RSB; return;}
        if(tokenChar == 40){tokenType = TOKEN_LRB; return;}
        if(tokenChar == 41){tokenType = TOKEN_RRB; return;}
        if(tokenChar == 123){tokenType = TOKEN_LCB; return;}
        if(tokenChar == 125){tokenType = TOKEN_RCB; return;}
        if(tokenChar == 59){tokenType = TOKEN_SEMICOLON; return;}
        if(tokenChar == 44){tokenType = TOKEN_COMMA; return;}
        if(tokenChar == 58){tokenType = TOKEN_COLON; return;}
        //if(tokenChar == '#'){tokenType = 510; return;}
        if(isLetter(tokenChar)>0)
        {
            tokenType = TOKEN_IDENTIFIER;
            stringValue[0] = tokenChar;
            stringValue[1] = 0;
            return;
        }
        if(isDigit(tokenChar)>0)
        {
            tokenType = TOKEN_CONSTINT;
            intValue = strToInt(status);
            return;
        }
    } // if(len == 1)
    else
    {
        if(len == 2)
        {
            if(strCompare(status, "==")>0){tokenType = TOKEN_EQUAL; return;}
            if(strCompare(status, "<=")>0){tokenType = TOKEN_LESSEQUAL; return;}
            if(strCompare(status, ">=")>0){tokenType = TOKEN_GREATEREQUAL; return;}
            if(strCompare(status, "<<")>0){tokenType = TOKEN_SHIFTLEFT; return;}
            if(strCompare(status, ">>")>0){tokenType = TOKEN_SHIFTRIGHT; return;}
            if(strCompare(status, "&&")>0){tokenType = TOKEN_AND; return;}
            if(strCompare(status, "||")>0){tokenType = TOKEN_OR; return;}
            if(strCompare(status, "!=")>0){tokenType = TOKEN_UNEQUAL; return;}
            if(strCompare(status, "if")>0){tokenType = TOKEN_IF; return;}
            if(strCompare(status, "->")>0){tokenType = TOKEN_ACCESS; return;}
            if(isLetter(status[0])>0)
            {
                tokenType = TOKEN_IDENTIFIER;
                strCopy(status, stringValue);
                return;
            }
            if(isDigit(status[0])>0)
            {
                tokenType = TOKEN_CONSTINT;
                intValue = strToInt(status);
                return;
            }
        } // if(len == 2)
        else
        {
            if(strCompare(status, "void")>0){tokenType = TOKEN_VOID; return;}
            if(strCompare(status, "int")>0){tokenType = TOKEN_INT; return;}
            if(strCompare(status, "char")>0){tokenType = TOKEN_CHAR; return;}
            if(strCompare(status, "double")>0){tokenType = TOKEN_DOUBLE; return;}
            if(strCompare(status, "while")>0){tokenType = TOKEN_WHILE; return;}
            if(strCompare(status, "else")>0){tokenType = TOKEN_ELSE; return;}
            if(strCompare(status, "return")>0){tokenType = TOKEN_RETURN; return;}
            if(strCompare(status, "struct")>0){tokenType = TOKEN_STRUCT; return;}
            if(strCompare(status, "static")>0){tokenType = TOKEN_STATIC; return;}
            if(strCompare(status, "#include")>0){tokenType = TOKEN_INCLUDE; return;}
            if(strCompare(status, "fopen")>0) {tokenType = TOKEN_FOPEN; return;}
            if(strCompare(status, "fclose")>0) {tokenType = TOKEN_FCLOSE; return;}
            if(strCompare(status, "fgetc")>0) {tokenType = TOKEN_FGETC; return;}
            if(strCompare(status, "fputc")>0) {tokenType = TOKEN_FPUTC; return;}
            if(strCompare(status, "malloc")>0) {tokenType = TOKEN_MALLOC; return;}
            if(strCompare(status, "sizeof")>0) {tokenType = TOKEN_SIZEOF; return;}
            if(strCompare(status, "printf")>0) {tokenType = TOKEN_PRINTF; return;}
            if(isLetter(status[0])>0)
            {
                tokenType = TOKEN_IDENTIFIER;
                strCopy(status, stringValue);
                return;
            }
            if(isDigit(status[0])>0){
                tokenType = TOKEN_CONSTINT;
                intValue = strToInt(status);
                return;
            }
            if(status[0] == 39) // Char literal
            {
                tokenType = TOKEN_CONSTCHAR;
                strTrimQuotes(status, stringValue);
                return;
            }
            if(status[0] == 34) // String literal
            {
                tokenType = TOKEN_STRING_LITERAL;
                strTrimQuotes(status, stringValue);
                return;
            }
          
        } // if(len >= 3)
    }
    
    tokenType = -1;
    
}

// --------------------------- Interface to scanner ---------------------------

void getNextToken()
{
    char * status;
    int len;
    int checkPeek;

    if(tokenType == TOKEN_EOF){
        return;
    }
    if(eofFlag == 1){
        tokenType = TOKEN_EOF;
        return;
    }

    
    status = malloc(sizeof(char) * 1024);
    
    len = 0;
    tokenType = -1;
    
    while(tokenType == -1){
    
        if(currentChar < 0) 
        {
            currentChar = readNextCharacter();
        }
        
        if(nextChar < 0) 
        {
            nextChar = readNextCharacter();
        }

        if((currentChar == -1) || (nextChar == -1))
        {
            // set token type to EOF
            tokenType = TOKEN_EOF;
            return;
        }

        // Trim whitespace
        if(isWhitespace(currentChar)>0){
            while(isWhitespace(currentChar)>0){
                currentChar = nextChar;
                nextChar = readNextCharacter();
            }
        }

        // Support for line comments
        if((currentChar == 47) && (nextChar == 47)){
            while ((currentChar != 10) && (currentChar != -1)){
                currentChar = nextChar;
                nextChar = readNextCharacter();
                if(nextChar == -1){
                    tokenType = TOKEN_EOF;
                    return;
                }
            }
        }
        else
        {
            positionColumn = col-2; // as two characters are read already
            positionLine = lin;
            
            
            checkPeek = peek(currentChar, nextChar);
            len = len +1;
            status[len - 1] = currentChar;
            if(nextChar > 0){
                currentChar = nextChar;
                nextChar = readNextCharacter();
            }
            
            while (checkPeek == 0){
                checkPeek = peek(currentChar, nextChar);
                len = len +1;
                status[len - 1] = currentChar;
                currentChar = nextChar;
                nextChar = readNextCharacter();
            }
            
            status[len] = 0;
            //Analize the token
            findToken(status, len);
            len = 0;
            // printf("%d",tokenType);
            // if(tokenType == 100 || tokenType == 300){
            //     printf(" (%s)",stringValue);
            // }
            // if(tokenType == 200){
            //     printf(" (%d)",intValue);
            // }
            // printf("\n");
            
            if(nextChar < 0){
                eofFlag = 1;
            }

        }
    }
}


int main(){
    initTokens();
    openFile("test/scanner.c");
    while(tokenType != TOKEN_EOF){
        getNextToken();
        printf(tokenType);
    }
    
}

