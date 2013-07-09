struct beidl_t {
	char * name;
};

void allocName(struct beidl_t * beidl)
{
	beidl = malloc(sizeof(struct beidl_t));
	beidl->name = malloc(sizeof(char) * 1000);
}

void main()
{
	struct beidl_t * beidl;
	
	beidl = malloc(sizeof(struct beidl_t));
	allocName(beidl);
}