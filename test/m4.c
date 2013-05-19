struct rec_t {
	    int f;
	    int g;
	    int *arr;
};

int a;
struct rec_t *rec;

void main() {
	

	a = 5;
	rec = malloc(sizeof(struct rec_t));
	rec->arr = malloc(sizeof(int) * 3);
	rec->arr[1] = 4;
	rec->f = 7;
	rec->g = 9;
	rec->arr[0] = a + (a - rec->arr[1]) * rec->f - a / rec->g * (12 - 11);
}