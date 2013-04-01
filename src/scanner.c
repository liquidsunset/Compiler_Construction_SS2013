#include <stdio.h> // used for tests
#include "scanner.h"

FILE *fp;

// ---------------------------- Tools -----------------------------------------

// Compares two character arrays.
// 
// Returns true if both arrays hold the same characters.
int strCompare(char a[], char b[]) // TODO: Support call by reference
{
	int i = 0;
	char ca = a[i];
	char cb = b[i];

	while( (i < 1024) && (a[i] != 0 || b[i] != 0) ) {
		if( ca != cb )
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
int strLength(char a[])
{
	int i=0;
	while(a[i]!='\0'&&i<1024) i++;
	return i;
}

// String copy
//
// Copies the first char array into the second
void strCopy(char from[], char to[])
{
	int i = 0;
	while(from[i] != 0 && i < 1023)
	{
		to[i]=from[i];
		i = i+1;
	}
	to[i]='\0';
}

void strTrimQuotes(char a[], char b[])
{
    int i = 0;

    do
    {
        b[i] = a[i+1];
        i = i +1;
    } while((a[i] != '\'' && a[i] != '\"') && a[i] != 0 && i < 1024);

    b[i-1] = 0;
}

// Checks if the character c is a letter.
// Umlauts are not supported, our input encoding is ASCII.
//
// Returns true if the character is within the range of [a-zA-Z]
int isLetter(char c)
{
	return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z');
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
    // String literals:
    static int isInString = 0;

    if(current == '\"' && !isInString) // add the starting "
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
    static int isInChar = 0;

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
	if(isTerminalChar(next)) return 1; // Brackets always terminate (as they are single char tokens)

    //if(current == '\'' || next == '\'') return 0; // char literals

    //if(current == '\"' && isInString)
    //{
    //    isInString = 1;
    //    return 0;
    //}
    

    if(isLetter(current) && isLetter(next)) return 0; //Letter letter: avg
    if(isDigit(current) && isDigit(next)) return 0; // Digit digit: 42
    if(isLetter(current) && isDigit(next)) return 0; // Letter digit: List1

    if (isOperator(current) && isOperator(next)) {return 0;}

	return 1;
}

int power(int base, int exp)
{
    int i = 0;
    int res = 1;
    while(i < exp)
    {
        res = res * base;
        i = i + 1;
    }
    return res;
}

int strToInt(char str[])
{
    int i = 0;
    int len = strLength(str);
    int res = 0;

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
void openFile(char path[]){

    fp = fopen(path,"r");
    if(fp == NULL)
    {
        tokenType = 509;
        fclose(fp);
    }
    
}

//Returns the next Character from the file
int readNextCharacter(){
    
    if(ftell(fp) < 0)
    {

        return EOF;
    }
    else
    {
        int temp = fgetc(fp);
        if(temp == EOF)
        {
            fclose(fp);
        }
        return temp;
    }
}



void findToken(char status[1024],int len){
    if(len == 1)
    {
        char tokenChar = status[0];
        if(tokenChar == '+'){tokenType = 400; return;}
        if(tokenChar == '-'){tokenType = 401; return;}
        if(tokenChar == '*'){tokenType = 402; return;}
        if(tokenChar == '/'){tokenType = 403; return;}
        if(tokenChar == '%'){tokenType = 404; return;}
        if(tokenChar == '='){tokenType = 405; return;}
        if(tokenChar == '<'){tokenType = 409; return;}
        if(tokenChar == '>'){tokenType = 410; return;}
        if(tokenChar == '&'){tokenType = 413; return;}
        if(tokenChar == '|'){tokenType = 415; return;}
        if(tokenChar == '^'){tokenType = 417; return;}
        if(tokenChar == '~'){tokenType = 418; return;}
        if(tokenChar == '!'){tokenType = 422; return;}
        if(tokenChar == '['){tokenType = 500; return;}
        if(tokenChar == ']'){tokenType = 501; return;}
        if(tokenChar == '('){tokenType = 502; return;}
        if(tokenChar == ')'){tokenType = 503; return;}
        if(tokenChar == '{'){tokenType = 504; return;}
        if(tokenChar == '}'){tokenType = 505; return;}
        if(tokenChar == ';'){tokenType = 506; return;}
        if(tokenChar == ','){tokenType = 507; return;}
        if(tokenChar == ':'){tokenType = 508; return;}
        if(tokenChar == '#'){tokenType = 510; return;}
        if(isLetter(tokenChar))
        {
            tokenType = 100;
            stringValue[0] = tokenChar;
            stringValue[1] = '\0';
            return;
        }
        if(isDigit(tokenChar))
        {
            tokenType = 200;
            intValue = strToInt(status);
            return;
        }
    } // if(len == 1)
    else if(len == 2)
    {
        if(strCompare(status, "==")){tokenType = 406; return;}
        if(strCompare(status, "<=")){tokenType = 407; return;}
        if(strCompare(status, ">=")){tokenType = 408; return;}
        if(strCompare(status, "<<")){tokenType = 411; return;}
        if(strCompare(status, ">>")){tokenType = 412; return;}
        if(strCompare(status, "&&")){tokenType = 414; return;}
        if(strCompare(status, "||")){tokenType = 416; return;}
        if(strCompare(status, "!=")){tokenType = 419; return;}
        if(strCompare(status, "++")){tokenType = 420; return;}
        if(strCompare(status, "--")){tokenType = 421; return;}
        if(strCompare(status, "if")){tokenType = 6; return;}
        if(isLetter(status[0]))
        {
            tokenType = 100;
            strCopy(status, stringValue);
            return;
        }
        if(isDigit(status[0]))
        {
            tokenType = 200;
            intValue = strToInt(status);
            return;
        }
    } // if(len == 2)
    else{
        if(strCompare(status, "NULL")){tokenType = 0; return;}
        if(strCompare(status, "void")){tokenType = 1; return;}
        if(strCompare(status, "int")){tokenType = 2; return;}
        if(strCompare(status, "char")){tokenType = 3; return;}
        if(strCompare(status, "double")){tokenType = 4; return;}
        if(strCompare(status, "while")){tokenType = 5; return;}
        if(strCompare(status, "else")){tokenType = 7; return;}
        if(strCompare(status, "return")){tokenType = 8; return;}
        if(strCompare(status, "struct")){tokenType = 9; return;}
        if(isLetter(status[0]))
        {
            tokenType = 100;
            strCopy(status, stringValue);
            return;
        }
        if(isDigit(status[0])){
            tokenType = 200;
            intValue = strToInt(status);
            return;
        }
        if(status[0] == '\'') // Char literal
        {
            tokenType = 202;
            strTrimQuotes(status, stringValue);
            return;
        }
        if(status[0] == '\"') // String literal
        {
            tokenType = 300;
            strTrimQuotes(status, stringValue);
            return;
        }
      
    } // if(len >= 3)
    
}

// --------------------------- Interface to scanner ---------------------------

void getNextToken()
{
    static int currentChar = -1;
    static int nextChar = -1;
    char status[1024];
    int len = 0;
    
    if(currentChar < 0) 
    {
        currentChar = readNextCharacter();
    }
    
    if(nextChar < 0) 
    {
        nextChar = readNextCharacter();
    }

    if(currentChar == EOF)
    {
        // set token type to EOF
        tokenType = 509;
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
        while ((char) currentChar != '\n' && currentChar != EOF){
            currentChar = nextChar;
            nextChar = readNextCharacter();
        }
    }
    else
    {
        int checkPeek;
        
        do
        {
            checkPeek = peek(currentChar, nextChar);
            len = len +1;
            status[len - 1] = currentChar;
            currentChar = nextChar;
            nextChar = readNextCharacter();                
        } while (checkPeek == 0);
        
        status[len] = '\0';
        //Analize the token
        findToken(status, len);
        len = 0;

    }        
}
// ----------------------------------------------------------------------------

// ------------------------ Tests ---------------------------------------------
// These tests should never be of priority for self-compilation.

int main()
{
    
    printf("\n\nNext Testfile- easy.c\n\n");
    
    openFile("../test/easy.c");
    if(tokenType != 509){
        do
        {
            getNextToken();
            printf("%d\n", tokenType);
            if(tokenType == 100){printf("%s\n", stringValue);}
            if(tokenType == 300){printf("%s\n", stringValue);}
            if(tokenType == 200){printf("%d\n", intValue);}
            if(tokenType == 202){printf("%s\n", stringValue);}
        }
        while(tokenType!= 509);
    }
    
    tokenType = -1;
    
    printf("\n\nNext Testfile - brackets.c\n\n");
    
    openFile("../test/brackets.c");
    if(tokenType != 509){
        do
        {
            getNextToken();
            printf("%d\n", tokenType);
            if(tokenType == 100){printf("%s\n", stringValue);}
            if(tokenType == 300){printf("%s\n", stringValue);}
            if(tokenType == 200){printf("%d\n", intValue);}
            if(tokenType == 202){printf("%s\n", stringValue);}
        }
        while(tokenType!= 509);
    }
    
    tokenType = -1;
    
    printf("\n\nNext Testfile - operator.c\n\n");
    
    openFile("../test/operator.c");
    if(tokenType != 509){
        do
        {
            getNextToken();
            printf("%d\n", tokenType);
            if(tokenType == 100){printf("%s\n", stringValue);}
            if(tokenType == 300){printf("%s\n", stringValue);}
            if(tokenType == 200){printf("%d\n", intValue);}
            if(tokenType == 202){printf("%s\n", stringValue);}
        }
        while(tokenType!= 509);
    }
    
    tokenType = -1;
    
    printf("\n\nNext Testfile - comments.c\n\n");
    
    openFile("../test/comments.c");
    if(tokenType != 509){
        do
        {
            getNextToken();
            printf("%d\n", tokenType);
            if(tokenType == 100){printf("%s\n", stringValue);}
            if(tokenType == 300){printf("%s\n", stringValue);}
            if(tokenType == 200){printf("%d\n", intValue);}
            if(tokenType == 202){printf("%s\n", stringValue);}
        }
        while(tokenType!= 509);
    }
    
    return 0;
    
}
// ----------------------------------------------------------------------------
