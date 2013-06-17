static int x;

int test(int x, int y, int * numbers);

void main()
{
	int * numbers;
	numbers = malloc(sizeof(int) * 3);
	numbers[0] = 0-5;
	numbers[1] = 0;
	numbers[2] = 5;
	x = 3;
	x = x + test(7, 5, numbers);
}

int test(int x, int y, int * numbers)
{
	int z;
	int i;
	i = 0;
	while(i < 3)
	{
		if(numbers[i] >= 0)
		{
			if(numbers[i] > 0)
			{
				printf(numbers[i]);
				printf(">0");
			}
			else
			{
				printf(numbers[i]);
				printf("=0");
			}
		}
		else
		{
			printf(numbers[i]);
			printf("<0");
		}
		i = i + 1;
	}
	// printf(numbers[0]);
	// printf(numbers[1]);
	// printf(numbers[2]);
	z = x+y;
	return z;
}