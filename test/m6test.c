int minus(int a)
{
    int z;
    if(a <= 0)
    {
        return 0;
    }
    else
    {
        z = minus(a-1);
        return 1;
    }

}

void main()
{
    int i;

    i = minus(1);
}