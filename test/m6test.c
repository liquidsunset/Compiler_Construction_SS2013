int minus(int a)
{
    if(a <= 0)
    {
        printf("Hit first return");
        return 0;
    }
    else
    {
        printf("Hit second return");
        return 1;
    }
}

void main()
{
    int i;

    i = minus(1);
}