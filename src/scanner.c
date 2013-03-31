#include <stdio.h> // used for tests
#include "scanner.h"

// ---------------------------- Tools -----------------------------------------

// Compares two character arrays.
// 
// Returns true if both arrays hold the same characters.
int strcmp(char a[], char b[]) // TODO: Support call by reference
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
int strlen(char a[])
{
	int i=0;
	while(a[i]!='\0'&&i<1024) i++;
	return i;
}

// String copy
//
// Copies the first char array into the second
void strcp(char from[], char to[])
{
	int i = 0;
	while(from[i] != 0 && i < 1023)
	{
		to[i]=from[i];
		i = i+1;
	}
	to[i]='\0';
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

int pow(int base, int exp)
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
    int len = strlen(str);
    int res = 0;

    if(str[0] == '-')
    {
        i = i + 1;
    }

    while(str[i] != 0 && i < 1024)
    {
        if(str[i] >= '0' && str[i] <= '9')
        {
            res = res + (str[i]-'0') * pow(10, len-i-1);
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

//Returns the next Character from the file
int readNextCharacter(){
    static FILE *fp = 0;

    if(fp == 0)
    {
        fp = fopen("../test/easy.c","r");
        //fp = fopen("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/test/easy.c","r");
    }
    int pos = ftell(fp);

    if(fp == NULL || pos < 0)
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
    if(len == 1){
        char tokenChar = status[0];
        if(tokenChar == '+'){
            tokenType = 400;
        }else if (tokenChar == '-'){
            tokenType = 401;
        }else if (tokenChar == '*'){
            tokenType = 402;
        }else if (tokenChar == '/'){
            tokenType = 403;
        }else if (tokenChar == '%'){
            tokenType = 404;
        }else if (tokenChar == '='){
            tokenType = 405;
        }else if (tokenChar == '<'){
            tokenType = 409;
        }else if (tokenChar == '>'){
            tokenType = 410;
        }else if (tokenChar == '&'){
            tokenType = 413;
        }else if (tokenChar == '|'){
            tokenType = 415;
        }else if (tokenChar == '^'){
            tokenType = 417;
        }else if (tokenChar == '~'){
            tokenType = 418;
        }else if (tokenChar == '!'){
            tokenType = 422;
        }else if (tokenChar == '['){
            tokenType = 500;
        }else if (tokenChar == ']'){
            tokenType = 501;
        }else if (tokenChar == '('){
            tokenType = 502;
        }else if (tokenChar == ')'){
            tokenType = 503;
        }else if (tokenChar == '{'){
            tokenType = 504;
        }else if (tokenChar == '}'){
            tokenType = 505;
        }else if (tokenChar == ';'){
            tokenType = 506;
        }else if (tokenChar == ','){
            tokenType = 507;
        }else if (tokenChar == ':'){
            tokenType = 508;
        }else if (tokenChar == '#'){
            tokenType = 510;
        }else if (isLetter(tokenChar)){
            tokenType = 100;
            stringValue[0] = tokenChar;
            stringValue[1] = '\0';
        }else if (isDigit(tokenChar)){
            tokenType = 200;
            tokenValue = strToInt(status);
        }
    }else if (len == 2){
        if(strcmp(status, "==")){
            tokenType = 406;
        }else if (strcmp(status, "<=")){
            tokenType = 407;
        }else if (strcmp(status, ">=")){
            tokenType = 408;
        }else if (strcmp(status, "<<")){
            tokenType = 411;
        }else if (strcmp(status, ">>")){
            tokenType = 412;
        }else if (strcmp(status, "&&")){
            tokenType = 414;
        }else if (strcmp(status, "||")){
            tokenType = 416;
        }else if (strcmp(status, "!=")){
            tokenType = 419;
        }else if (strcmp(status, "++")){
            tokenType = 420;
        }else if (strcmp(status, "--")){
            tokenType = 421;
        }else if (strcmp(status, "if")){
            tokenType = 6;
        }else if (isLetter(status[0])){
            tokenType = 100;
            strcp(status, stringValue);
        }else if (isDigit(status[0])){
            tokenType = 200;
            tokenValue = strToInt(status);
        }
    }else{
        if(strcmp(status, "NULL")){
            tokenType = 0;
        }else if (strcmp(status, "void")){
            tokenType = 1;
        }else if (strcmp(status, "int")){
            tokenType = 2;
        }else if (strcmp(status, "char")){
            tokenType = 3;
        }else if (strcmp(status, "double")){
            tokenType = 4;
        }else if (strcmp(status, "while")){
            tokenType = 5;
        }else if (strcmp(status, "else")){
            tokenType = 7;
        }else if (strcmp(status, "return")){
            tokenType = 8;
        }else if (strcmp(status, "struct")){
            tokenType = 9;
        }else if(isLetter(status[0])){
            tokenType = 100;
            strcp(status, stringValue);
        }else if(isDigit(status[0])){
            tokenType = 200;
            tokenValue = strToInt(status);
        }
      
    }
    
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
        len = 0;

        // TODO: analyse token

        printf("'%s'\n",status);
    }        
}
// ----------------------------------------------------------------------------

// ------------------------ Tests ---------------------------------------------
// These tests should never be of priority for self-compilation.

void main()
{
    char c= 'a';
    do
    {
        getNextToken();
        c = getchar();
    }
    while(c != 'x');
}
// ----------------------------------------------------------------------------