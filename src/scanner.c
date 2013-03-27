// ---------------------------- Tools -----------------------------------------

// TODO(allesmi): The following functions return an int used as a boolean
// 	A #define TRUE 1 or a enum could introduce proper boolean type.

// Compares two character arrays.
// 
// Returns true if both arrays hold the same characters.
int strcmp(char[1024] a, char[1024] b) // TODO: Support call by reference
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
int isCharacter(char c)
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