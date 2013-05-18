
#include <stdio.h> // used for tests
#include "globals.c"

static FILE *fp;

static int tokenType;
static int intValue;
static char stringValue[1024];
static int positionLine;
static int positionColumn;

static int lin;
static int col;

// ---------------------------- Tools -----------------------------------------

// Compares two character arrays.
// 
// Returns true if both arrays hold the same characters.
int strCompare(char a[1024], char b[1024]) // TODO: Support call by reference
{
	int i;
    i = 0;

	while( (i < 1024) && (a[i] != 0 || b[i] != 0) ) {
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
int strLength(char a[1024])
{
	int i;
    i = 0;
	while(a[i]!='\0'&&i<1024) i=i+1;
	return i;
}

// String copy
//
// Copies the first char array into the second
void strCopy(char from[1024], char to[1024])
{
	int i;
    i = 0;
	while(from[i] != 0 && i < 1023)
	{
		to[i]=from[i];
		i = i+1;
	}
	to[i]='\0';
}

void strTrimQuotes(char a[1024], char b[1024])
{
    int i;
    i = 0;
    
    b[i] = a[i+1];
    i = i +1;
    
    while((a[i] != '\'' && a[i] != '\"') && a[i] != 0 && i < 1024){
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
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || (c == '_');
}

// Checks if the character c is a digit.
//
// Returns true if the character is within the range of [0-9]
int isDigit(char c)
{
	return (c >= '0' && c <= '9');
}

// Checks if the character c is a whitespace:
// Space, Tabs, CR, LF.
//
// Returns true if the character is whitespace.
int isWhitespace(char c)
{
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

int isTerminalChar(char c)
{
	return (
        c == '(' ||
        c == ')' ||
        c == '[' ||
        c == ']' ||
        c == '{' ||
        c == '}' ||
        c == ';' ||
        c == ',' ||
        c == '\'' ||
        c == '\"'
        );
}

int isOperator(char c)
{
	return (!isWhitespace(c) && !isLetter(c) && !isDigit(c));
}

int characterClass(char c)
{
	if(isDigit(c)) return 1;
	if(isLetter(c)) return 2;

	return 3; // Terminal character
}
// ----------------------------------------------------------------------------

// ------------------------ Lexical analysis ----------------------------------

// Returns:
// 	0 if current token continues
// 	1 if it is terminated
int peek(int current, int next)
{


    if(current == '\"' && !isInChar && !isInString) // add the starting "
    {
        isInString = 1;
        return 0;
    }
    if(next == '\"' && isInString) return 0; // add the ending "
    if(current == '\"' && isInString) // terminate after the ending "
    {
        isInString = 0;
        return 1;
    }
    if(isInString) return 0;

    // Char literals (duplicated to support something like "'a'"):

    if(current == '\'' && !isInChar) // add the starting "
    {
        isInChar = 1;
        return 0;
    }
    if(next == '\'' && isInChar) return 0; // add the ending "
    if(current == '\'' && isInChar) // terminate after the ending "
    {
        isInChar = 0;
        return 1;
    }
    if(isInChar) return 0;

    if(current < 0 || next < 0) return 1; // EOF
	if(isWhitespace(next)) return 1; // Whitespace always terminates.
	if(isTerminalChar(current) || isTerminalChar(next)) return 1; // Brackets always terminate (as they are single char tokens)

    //if(current == '\'' || next == '\'') return 0; // char literals

    //if(current == '\"' && isInString)
    //{
    //    isInString = 1;
    //    return 0;
    //}
    if(current == '#') return 0;

    if(isLetter(current) && isLetter(next)) return 0; //Letter letter: avg
    if(isDigit(current) && isDigit(next)) return 0; // Digit digit: 42
    if(isLetter(current) && isDigit(next)) return 0; // Letter digit: List1

    if (isOperator(current) && isOperator(next)) {return 0;}

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

int strToInt(char str[1024])
{
    int i;
    int len;
    int res;
    i = 0;
    len = strLength(str);
    res = 0;

    if(str[0] == '-')
    {
        i = i + 1;
    }

    while(str[i] != 0 && i < 1024)
    {
        if(str[i] >= '0' && str[i] <= '9')
        {
            res = res + (str[i]-'0') * power(10, len-i-1);
        }
        i = i + 1;
    }

    if(str[0] == '-')
    {
        res = -res;
    }

    return res;
}
// ----------------------------------------------------------------------------

// creates the Filepointer
void openFile(char path[1024]){

    lin = 1;
    col = 1;
    fp = fopen(path,"r");
}

//Returns the next Character from the file
int readNextCharacter(){
    
    
//    if(ftell(fp) < 0)
//    {
//
//        return EOF;
//    }
//    else
//    {
    
        if(fp == 0){
            return EOF;
        }
    
    
        int temp;
        temp = fgetc(fp);

        if(temp < 0)
        {
            fclose(fp);
            return EOF;
        }
        col = col + 1;
        if(temp == '\n')
        {
            col = 1;
            lin = lin + 1;
            printf("\n");
        }

        return temp;
    //}
}



void findToken(char status[1024],int len){
    if(len == 1)
    {
        char tokenChar;
        tokenChar = status[0];
        if(tokenChar == '+'){tokenType = TOKEN_PLUS; return;}
        if(tokenChar == '-'){tokenType = TOKEN_MINUS; return;}
        if(tokenChar == '*'){tokenType = TOKEN_MULT; return;}
        if(tokenChar == '/'){tokenType = TOKEN_DIVIDE; return;}
        if(tokenChar == '%'){tokenType = TOKEN_PERCENT; return;}
        if(tokenChar == '='){tokenType = TOKEN_ASSIGNMENT; return;}
        if(tokenChar == '<'){tokenType = TOKEN_LESS; return;}
        if(tokenChar == '>'){tokenType = TOKEN_GREATER; return;}
        if(tokenChar == '&'){tokenType = TOKEN_ADDRESS; return;}
        if(tokenChar == '|'){tokenType = TOKEN_BITWISEOR; return;}
        if(tokenChar == '^'){tokenType = TOKEN_BITWISEEXCLOR; return;}
        if(tokenChar == '~'){tokenType = TOKEN_BITWISENOT; return;}
        if(tokenChar == '!'){tokenType = TOKEN_NOT; return;}
        if(tokenChar == '['){tokenType = TOKEN_LSB; return;}
        if(tokenChar == ']'){tokenType = TOKEN_RSB; return;}
        if(tokenChar == '('){tokenType = TOKEN_LRB; return;}
        if(tokenChar == ')'){tokenType = TOKEN_RRB; return;}
        if(tokenChar == '{'){tokenType = TOKEN_LCB; return;}
        if(tokenChar == '}'){tokenType = TOKEN_RCB; return;}
        if(tokenChar == ';'){tokenType = TOKEN_SEMICOLON; return;}
        if(tokenChar == ','){tokenType = TOKEN_COMMA; return;}
        if(tokenChar == ':'){tokenType = TOKEN_COLON; return;}
        //if(tokenChar == '#'){tokenType = 510; return;}
        if(isLetter(tokenChar))
        {
            tokenType = TOKEN_IDENTIFIER;
            stringValue[0] = tokenChar;
            stringValue[1] = '\0';
            return;
        }
        if(isDigit(tokenChar))
        {
            tokenType = TOKEN_CONSTINT;
            intValue = strToInt(status);
            return;
        }
    } // if(len == 1)
    else if(len == 2)
    {
        if(strCompare(status, "==")){tokenType = TOKEN_EQUAL; return;}
        if(strCompare(status, "<=")){tokenType = TOKEN_LESSEQUAL; return;}
        if(strCompare(status, ">=")){tokenType = TOKEN_GREATEREQUAL; return;}
        if(strCompare(status, "<<")){tokenType = TOKEN_SHIFTLEFT; return;}
        if(strCompare(status, ">>")){tokenType = TOKEN_SHIFTRIGHT; return;}
        if(strCompare(status, "&&")){tokenType = TOKEN_AND; return;}
        if(strCompare(status, "||")){tokenType = TOKEN_OR; return;}
        if(strCompare(status, "!=")){tokenType = TOKEN_UNEQUAL; return;}
        if(strCompare(status, "if")){tokenType = TOKEN_IF; return;}
        if(strCompare(status, "->")){tokenType = TOKEN_ACCESS; return;}
        if(isLetter(status[0]))
        {
            tokenType = TOKEN_IDENTIFIER;
            strCopy(status, stringValue);
            return;
        }
        if(isDigit(status[0]))
        {
            tokenType = TOKEN_CONSTINT;
            intValue = strToInt(status);
            return;
        }
    } // if(len == 2)
    else{
        if(strCompare(status, "void")){tokenType = TOKEN_VOID; return;}
        if(strCompare(status, "int")){tokenType = TOKEN_INT; return;}
        if(strCompare(status, "char")){tokenType = TOKEN_CHAR; return;}
        if(strCompare(status, "double")){tokenType = TOKEN_DOUBLE; return;}
        if(strCompare(status, "while")){tokenType = TOKEN_WHILE; return;}
        if(strCompare(status, "else")){tokenType = TOKEN_ELSE; return;}
        if(strCompare(status, "return")){tokenType = TOKEN_RETURN; return;}
        if(strCompare(status, "struct")){tokenType = TOKEN_STRUCT; return;}
        if(strCompare(status, "static")){tokenType = TOKEN_STATIC; return;}
        if(strCompare(status, "#include")){tokenType = TOKEN_INCLUDE; return;}
        if(strCompare(status, "fopen")) {tokenType = TOKEN_FOPEN; return;}
        if(strCompare(status, "fclose")) {tokenType = TOKEN_FCLOSE; return;}
        if(strCompare(status, "fgetc")) {tokenType = TOKEN_FGETC; return;}
        if(strCompare(status, "fputc")) {tokenType = TOKEN_FPUTC; return;}
        if(strCompare(status, "malloc")) {tokenType = TOKEN_MALLOC; return;}
        if(strCompare(status, "sizeof")) {tokenType = TOKEN_SIZEOF; return;}
        if(isLetter(status[0]))
        {
            tokenType = TOKEN_IDENTIFIER;
            strCopy(status, stringValue);
            return;
        }
        if(isDigit(status[0])){
            tokenType = TOKEN_CONSTINT;
            intValue = strToInt(status);
            return;
        }
        if(status[0] == '\'') // Char literal
        {
            tokenType = TOKEN_CONSTCHAR;
            strTrimQuotes(status, stringValue);
            return;
        }
        if(status[0] == '\"') // String literal
        {
            tokenType = TOKEN_STRING_LITERAL;
            strTrimQuotes(status, stringValue);
            return;
        }
      
    } // if(len >= 3)
    
    tokenType = -1;
    
}

// --------------------------- Interface to scanner ---------------------------

void getNextToken()
{
    if(tokenType == TOKEN_EOF){
        return;
    }

    char status[1024];
    int len;
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

        if(currentChar == EOF || nextChar == EOF)
        {
            // set token type to EOF
            tokenType = TOKEN_EOF;
            return;
        }

        // Trim whitespace
        if(isWhitespace(currentChar)){
            while(isWhitespace(currentChar)){
                currentChar = nextChar;
                nextChar = readNextCharacter();
            }
        }

        // Support for line comments
        if(currentChar == '/' && nextChar == '/'){
            while (currentChar != '\n' && currentChar != EOF){
                currentChar = nextChar;
                nextChar = readNextCharacter();
            }
        }
        else
        {
            int checkPeek;
            positionColumn = col-2; // as two characters are read already
            positionLine = lin;
            
            
            checkPeek = peek(currentChar, nextChar);
            len = len +1;
            status[len - 1] = currentChar;
            currentChar = nextChar;
            nextChar = readNextCharacter();
            
            while (checkPeek == 0){
                checkPeek = peek(currentChar, nextChar);
                len = len +1;
                status[len - 1] = currentChar;
                currentChar = nextChar;
                nextChar = readNextCharacter();
            }
            
            status[len] = '\0';
            //Analize the token
            findToken(status, len);
            len = 0;
            printf("%d",tokenType);
            if(tokenType == 100 || tokenType == 300){
                printf(" (%s)",stringValue);
            }
            printf("\t");

        }
    }
}

// ----------------------------------------------------------------------------

// ------------------------ Tests ---------------------------------------------
// These tests should never be of priority for self-compilation.

//int main()
//{
//    initTokens();
//    
//    printf("\n\nNext Testfile- easy.c\n\n");
//    
//    //openFile("../test/easy.c");
//    openFile("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/test/m4.c");
//    tokenType = -1;
//    if(tokenType != TOKEN_EOF){
//        do
//        {
//            getNextToken();
//            printf("Line %d:%d %d\n", positionLine, positionColumn, tokenType);
//            if(tokenType == TOKEN_IDENTIFIER){printf("%s\n", stringValue);}
//            if(tokenType == TOKEN_STRING_LITERAL){printf("%s\n", stringValue);}
//            if(tokenType == TOKEN_CONSTINT){printf("%d\n", intValue);}
//            if(tokenType == TOKEN_CONSTCHAR){printf("%s\n", stringValue);}
//        }
//        while(tokenType!= TOKEN_EOF);
//        getchar();
//    }
//}
//
//    
//    printf("\n\nNext Testfile - brackets.c\n\n");
//    
//    openFile("../test/brackets.c");
//    
//        do
//        {
//            getNextToken();
//            printf("%d\n", tokenType);
//            if(tokenType == TOKEN_IDENTIFIER){printf("%s\n", stringValue);}
//            if(tokenType == TOKEN_STRING_LITERAL){printf("%s\n", stringValue);}
//            if(tokenType == TOKEN_CONSTINT){printf("%d\n", intValue);}
//            if(tokenType == TOKEN_CONSTCHAR){printf("%s\n", stringValue);}
//        }
//        while(tokenType!= TOKEN_EOF);
//    
//    
//    printf("\n\nNext Testfile - operator.c\n\n");
//
//    //openFile("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/src/scanner.c");
//    openFile("../test/operator.c");
//
//        do
//        {
//            getNextToken();
//            printf("%d\n", tokenType);
//            if(tokenType == TOKEN_IDENTIFIER){printf("%s\n", stringValue);}
//            if(tokenType == TOKEN_STRING_LITERAL){printf("%s\n", stringValue);}
//            if(tokenType == TOKEN_CONSTINT){printf("%d\n", intValue);}
//            if(tokenType == TOKEN_CONSTCHAR){printf("%s\n", stringValue);}
//        }
//        while(tokenType!= TOKEN_EOF);
//
//    printf("\n\nNext Testfile - comments.c\n\n");
//    
//    openFile("../test/comments.c");
//        do
//        {
//            getNextToken();
//            printf("%d\n", tokenType);
//            if(tokenType == TOKEN_IDENTIFIER){printf("%s\n", stringValue);}
//            if(tokenType == TOKEN_STRING_LITERAL){printf("%s\n", stringValue);}
//            if(tokenType == TOKEN_CONSTINT){printf("%d\n", intValue);}
//            if(tokenType == TOKEN_CONSTCHAR){printf("%s\n", stringValue);}
//        }
//        while(tokenType!= TOKEN_EOF);
//    
//    
//    printf("\n\nNext Testfile - Scanner selfscanc\n\n");
//    
//    openFile("../src/scanner.c");
//    do
//    {
//        getNextToken();
//        printf("%d\n", tokenType);
//        if(tokenType == TOKEN_IDENTIFIER){printf("%s\n", stringValue);}
//        if(tokenType == TOKEN_STRING_LITERAL){printf("%s\n", stringValue);}
//        if(tokenType == TOKEN_CONSTINT){printf("%d\n", intValue);}
//        if(tokenType == TOKEN_CONSTCHAR){printf("%s\n", stringValue);}
//    }
//    while(tokenType!= TOKEN_EOF);
//    
//    
//    return 0;
//    
//    
//
//}
// ----------------------------------------------------------------------------
