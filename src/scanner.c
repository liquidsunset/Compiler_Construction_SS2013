#include <stdio.h> // used for tests
#include "globals.c"

static int * fp;

static int tokenType;
static int intValue;
static int positionLine;
static int positionColumn;
static int eofFlag;

static int lin;
static int col;

// String literals:
static int isInString;
static int isInChar;

static int currentChar;
static int nextChar;

// ---------------------------- Tools -----------------------------------------

// Compares two character arrays.
// 
// Returns true if both arrays hold the same characters.
int strCompare(char *a, char *b) // TODO: Support call by reference
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
    else
    {
        return 0;
    }
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
    else
    {
        return 0;
    }
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
    else
    {
        return 0;
    }
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
        (c == 39 ) ||
        (c == 34 )
        )
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

int isOperator(char c)
{
	if( !(isWhitespace(c)>0) && !(isLetter(c)>0) && !(isDigit(c)>0) )
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

    if((current == 34) && !(isInChar > 0) && !(isInString > 0)) // add the starting "
    {
        isInString = 1;
        return 0;
    }
    if((next == 34) && (isInString > 0)) {return 0;} // add the ending "
    if((current == 34) && (isInString > 0)) // terminate after the ending "
    {
        isInString = 0;
        return 1;
    }
    if(isInString > 0) {return 0;}

    // Char literals (duplicated to support something like "'a'"):
    if((current == 39) && !(isInChar > 0)) // add the starting "
    {
        isInChar = 1;
        return 0;
    }

    if((next == 39) && (isInChar > 0)) {return 0;} // add the ending "
    if((current == 39) && (isInChar > 0)) // terminate after the ending "
    {
        isInChar = 0;
        return 1;
    }
    if(isInChar > 0) {return 0;}

    

    if((current < 0) || (next < 0)) {return 1;} // EOF
	if(isWhitespace(cnext) > 0) {return 1;} // Whitespace always terminates.
	if((isTerminalChar(ccurrent) > 0) || (isTerminalChar(cnext) > 0)) {return 1;} // Brackets always terminate (as they are single char tokens)

    //if(current == '\'' || next == '\'') return 0; // char literals

    //if(current == '\"' && isInString)
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
    i = 0;
    len = strLength(str);
    res = 0;

    if(str[0] == '-')
    {
        i = i + 1;
    }

    while((str[i] != 0) && (i < 1024))
    {
        if((str[i] >= '0') && (str[i] <= '9'))
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
void openFile(char *path){

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
            //printf("\n");
        }

        return temp;
    //}
}



void findToken(char *status,int len){
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
        if(strCompare(status, "printf")) {tokenType = TOKEN_PRINTF; return;}
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
    if(eofFlag == 1){
        tokenType = TOKEN_EOF;
        return;
    }

    char *status;
    status = malloc(sizeof(char) * 1024);
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

        if((currentChar == EOF) || (nextChar == EOF))
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
        if((currentChar == '/') && (nextChar == '/')){
            while ((currentChar != '\n') && (currentChar != EOF)){
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
            int checkPeek;
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
            
            status[len] = '\0';
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

// ----------------------------------------------------------------------------

// ------------------------ Tests ---------------------------------------------
// These tests should never be of priority for self-compilation.
//
//int main()
//{
//    initTokens();
//    
//    printf("\n\nNext Testfile- easy.c\n\n");
//    
//    openFile("test/easy.c");
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
//        //getchar();
//    }
//}
// ----------------------------------------------------------------------------
