#include <stdio.h> // used for tests


int tokenValue;
char stringValue[1024];

// ---------------------------- Tools -----------------------------------------

// Compares two character arrays.
// 
// Returns true if both arrays hold the same characters.
int strcmp(char a[], char b[]) // TODO: Support call by reference
{
	int i = 0;
	char ca = a[i];
	char cb = b[i];

	while( (i < 1024) && (ca != 0 || cb != 0) ) {
		if( ca != cb )
		{
			return 0;
		}

		i = i + 1;
		ca = a[i];
		cb = b[i];
	}

	return 1;
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
	return (c == ' ' || c == 10 || c == 13 || c == '\t');
}

// ----------------------------------------------------------------------------

//Returns the next Character from the file
int readNextCharacter(){
    static FILE *fp = 0;
    
    if(fp == 0){
        fp = fopen("scanner.c","r");
    }
    
    if(fp == NULL){
        return 0;
    }
    else{
        int temp = fgetc(fp);
        if(temp == EOF){
            fclose(fp);
        }
        return temp;
    }
}

void getNextToken(){

}

int peek(char status[], int len, char nextChar){
    
    
}

// ------------------------ Tests ---------------------------------------------
// These tests should never be of priority for self-compilation.

void main()
{
    /*
    struct token {
        int tokenType;
        char charValue;
        int intValue;
        char charValueArray[1024];
    };
    */
    
    
	// --- Happy case:
	char letter = 'a', digit='1', whitespace=' ';

	if(isLetter(letter))
		printf("'%c' is a letter\n", letter);
	else
		printf("ERROR: '%c' should be a letter.\n", digit);

	if(isDigit(digit))
		printf("'%c' is a digit\n", digit);
	else
		printf("ERROR: '%c' should be a digit.\n", digit);

	if(isWhitespace(whitespace))
		printf("'%c' is a whitespace\n", whitespace);
	else
		printf("ERROR: '%c' should be a whitespace.\n", whitespace);

	// --- Sad cases:

	if(isLetter(digit))
		printf("ERROR: '%c' should not be a letter\n", digit);
	else
		printf("'%c' is not a letter.\n", digit);

	if(isDigit(whitespace))
		printf("ERROR: '%c' should not be a digit\n", whitespace);
	else
		printf("'%c' is not a digit.\n", whitespace);

	if(isWhitespace(letter))
		printf("ERROR: '%c' should not be a whitespace\n", letter);
	else
		printf("'%c' is not a whitespace.\n", letter);

}

// ----------------------------------------------------------------------------