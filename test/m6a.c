static int x;

int test(int x, int y)
{
	int z;
	printf(x);
	printf(y);
	z = x+y;
	printf(z);
	return z;
}

void main()
{
	x = 3;
	x = x + test(7, 5);
}