static int fp;
static char * typeName;
static int * intArr;

void openFile(char * filename)
{
	fp = fopen(filename);
}

void main()
{
	int c;

	
	openFile("test/scanner.c");
	c = fgetc(fp);
	while(c > -1)
	{
		printf(c);
		c = fgetc(fp);
	}
	fclose(fp);
}