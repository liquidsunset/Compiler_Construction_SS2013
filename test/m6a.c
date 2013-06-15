static int x;

int test(int x, int y)
{
	int z;
	z = x+y;
	return z;
}

void main()
{
	x = 3;
	x = x + test(7, 5);
}