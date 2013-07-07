/* stringpassing.c */

static char * stringValue;

void print(char * string)
{
	printf(string);
}



void main()
{
	stringValue = malloc(sizeof(char) * 5);
	stringValue[0] = 65;
	stringValue[1] = 66;
	stringValue[2] = 67;
	stringValue[3] = 68;
	stringValue[4] = 0;
	print(stringValue);
	print("Welt");
}