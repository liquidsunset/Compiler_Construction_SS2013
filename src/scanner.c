#include <stdio.h> // used for tests

int tokenType;
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
	return (c == ' ' || c == '\n' || c == '\r' || c == '\t');
}

int isTerminalChar(char c)
{
	return (c == '(' || c == ')' || c == '[' || c == ']' || c == '{' || c == '}' || c == ';');
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
int peek(char current, char next)
{
	if(isWhitespace(next)) return 1; // Whitespace always terminates.
	if(isTerminalChar(next)) return 1; // Brackets always terminate (as they are single char tokens)

	if(!isOperator(current) && !isOperator(next)) return 0; // Letter/digit followed by letter/digit
															// Examples:
															// Letter letter: avg
															// Letter digit: List1
															// Digit letter: 1001b
															// Digit digit: 42
	if(isOperator(current) && !isOperator(next)) return 1;
    if (isOperator(current) && isOperator(next)) return 0;
	return 1;
}
// ----------------------------------------------------------------------------

//Returns the next Character from the file
int readNextCharacter(){
    static FILE *fp = 0;
    
    if(fp == 0){
        fp = fopen("/Users/liquidsunset/Documents/Angewandte_Informatik/4. Semester/Compilerbau/Phoenix/src/scanner.c","r");
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

    int currentChar = readNextCharacter();
    int nextChar = readNextCharacter();
    char status[1024];
    int len = 0;
    
    
    while(currentChar != EOF){
        

        if(isWhitespace(currentChar)){
            while(isWhitespace(currentChar)){
                currentChar = nextChar;
                nextChar = readNextCharacter();
            }
        }

        
        if(currentChar == '/' && nextChar == '/'){
            while ((char) currentChar != '\n' && currentChar != EOF){
                currentChar = nextChar;
                nextChar = readNextCharacter();
            }
        }
        else
        {
        

            int checkPeek;
            
            do{
                checkPeek = peek((char) currentChar, (char) nextChar);
                len = len +1;
                status[len - 1] = currentChar;
                currentChar = nextChar;
                nextChar = readNextCharacter();
                
                
            }while (checkPeek == 0);
            
            status[len] = '\0';
            len = 0;
            
            

            
            printf("'%s'\n",status);
        
        }

        
    }
            
}


void findToken(char status[1024], len){
    if(len == 1){
        char tokenChar = status[0];
        if(tokenChar == '+'){
            
        }
            
        
    }
    
}


// ------------------------ Tests ---------------------------------------------
// These tests should never be of priority for self-compilation.

void main()
{
    
    getNextToken();
    
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