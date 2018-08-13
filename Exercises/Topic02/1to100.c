#include <stdio.h>
#include <math.h>
// Use a FOR statement to count from 1 to 100. 

#define MIN 1
#define MAX 100

int main(void) { 
    printf("Counting from %d to %d", MIN, MAX); 
    for ( int i = MIN; i <= MAX; i++ ) { 
        printf("%d\n", i); 
    } 
}