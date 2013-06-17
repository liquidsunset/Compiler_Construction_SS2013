int even (int num);
int odd (int num);
void even_or_odd (int *numbers, int len);

int main () {
    int *numbers;//[3] = { -2, 0, 7 };
    int len;
    len = 3;
    
    numbers = malloc(sizeof(int)*len);
    numbers[0] = 0-2;
    numbers[1] = 0;
    numbers[2] = 7;
    
    even_or_odd(numbers, len);
}

void even_or_odd (int *numbers, int len) {
    int i;
    i = 0;
    if(1 != 1)
    {
    	printf("ERROR");
    	printf("ERROR");
    	printf("ERROR");
    	printf("ERROR");
    	
    }
    else
    {
    	printf("OK");
    	printf("OK");
    	printf("OK");
    	printf("OK");
    	
    }
    // while (i < len) {
    //     if ( (numbers[i] > 0))
    //     {
    //     	printf(numbers[i]);
    //     	printf("Ist > 0");
    //     }
    //     else
    //     {
	   //      if ( (numbers[i] == 0))
	   //      {
	   //      	printf(numbers[i]);
	   //      	printf("Ist <=0 und 0");
	   //      }
	   //      else
	   //      {

		  //       if ( (numbers[i] == 7))
		  //       {
		  //       	printf(numbers[i]);
		  //       	printf("Ist <=0 und 7");
		  //       }
	   //  	}
	   //  }
    //     i = i + 1;
    // }
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