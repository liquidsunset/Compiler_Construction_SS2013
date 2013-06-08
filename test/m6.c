int even (int num);
int odd (int num);
void even_or_odd (int *numbers, int len);

int main (int argc, char **argv) {
    int *numbers;//[3] = { -2, 0, 7 };
    int len = 3;
    
    numbers = malloc(sizeof(int)*len);
    numbers[0] = -2;
    numbers[1] = 0;
    numbers[2] = 7;
    
    even_or_odd(numbers, len);
}

void even_or_odd (int *numbers, int len) {
    int i = 0;
    while (i < len) {
        if ((numbers[i] >= 0) && even(numbers[i] > 0)) {
            printf("%d is even\n", numbers[i]);
        } else if ((numbers[i] >= 0) && odd(numbers[i] > 0)) {
            printf("%d is odd\n", numbers[i]);
        } else {
            printf("number is < 0, sorry...\n");
        }
        i = i + 1;
    }
    printf("end");
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