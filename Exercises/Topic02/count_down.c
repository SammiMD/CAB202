#include <stdio.h>

void count_down( void ) {
	// (a) Print the introductory message.
    printf("Counting backwards in steps of 7, beginning at 315, stopping before 93.\n");
    
	// (b) Declare an integer variable that you will use to count. Initially 
	//     the counter should equal the start value, 315.
    int i = 315;

	// (c) Begin a WHILE statement that will execute its loop body if the 
	//     counter is greater than the end value, 93.
    while (i>93) {
        printf("%d\n", i);
        i = i-7;
    }

		// (e) Print the value of the counter on a line by itself.

		// (f) Subtract the step size, 7, from the counter.
	// (d) End the WHILE statement
}

int main( void ) {
	count_down();
	return 0;
}