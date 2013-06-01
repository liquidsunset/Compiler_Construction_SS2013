struct rec_t {
    int *data;
    int max;
    int sum;
    int avg;
};
static int i;
static int len;
static struct rec_t *rec;


void main(){
    
    i = 0;
    len = 10; //try some different positive values here

    rec = malloc(sizeof(struct rec_t));
    rec->max = 0;
    rec->sum = 0;
    rec->avg = 42;
    rec->data = malloc(sizeof(int) * len);

    // statically initialize rec->data with 'len' values, e.g.,
    // rec->data[0] = 23; ...; rec->data[len-1] = 42;
    
    rec->data[0] = 0;
    rec->data[1] = 1;
    rec->data[2] = 2;
    rec->data[3] = 3;
    rec->data[4] = 4;
    rec->data[5] = 5;
    rec->data[6] = 6;
    rec->data[7] = 7;
    rec->data[8] = 8;
    rec->data[9] = 9;
    // rec->data[10] = 10;
    // rec->data[11] = 11;
    // rec->data[12] = 12;
    // rec->data[13] = 13;
    // rec->data[14] = 14;
    // rec->data[15] = 15;
    // rec->data[16] = 16;
    // rec->data[17] = 17;
    // rec->data[18] = 18;
    // rec->data[19] = 19;

    while (i < len) {
        rec->sum = rec->sum + rec->data[i];
        if (rec->data[i] > rec->max) {
            rec->max = rec->data[i];
        }
        i = i + 1;
    }

    if ((len != 0) && (rec->sum / len > 0)) {
        rec->avg = rec->sum / len;
    } else {
        rec->avg = 0;
    }
}
