void main()
{
	int fp;
	int c;

	fp = fopen("test.txt");
	c = fgetc(fp);
	while(c > -1)
	{
		printf(c);
		c = fgetc(fp);
	}
	fputc(10, fp);
	fputc(65, fp);
	fputc(10, fp);
	fclose(fp);
}