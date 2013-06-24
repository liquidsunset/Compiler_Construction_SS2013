//DEMO
int fib(int n);
int even(int num);
int odd(int odd);

struct array_t
{
	int * numbers;
	int len;
};

static struct array_t * array;

void main()
{
    int offset;
	int i;

	array = malloc(sizeof(struct array_t));
	array->len = 5;
	array->numbers = malloc(array->len * sizeof(int));
    
	array->numbers[0] = fib(0);
	array->numbers[1] = fib(1);
	array->numbers[2] = fib(2);
	array->numbers[3] = fib(3);
	array->numbers[4] = fib(4);
    
	i = 0;
	while(i < array->len)
	{
		printf(i);
		printf(". Fibonacci-Zahl: ");
        
		if(even(array->numbers[i]) > 0)
		{
			printf(array->numbers[i]);
			printf(" ist gerade.");
		}
		else
		{
			printf(array->numbers[i]);
			printf(" ist ungerade.");
		}
		i = i + 1;
	}
}

int fib(int n)
{
	int a;
	int b;

	if(n <= 1)
	{
		return n;
	}
    a = fib(n-2);
    b = fib(n-1);

	return a+b;
}

int even (int num) {
    if (num == 0) {
        return 1;
    } else {
        return odd(num-1);
    }
}

int odd (int num) {
    if (num == 0) {
        return 0;
    } else {
        return even(num-1);
    }
}