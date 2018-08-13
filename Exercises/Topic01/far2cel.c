#include <stdio.h>
#include <math.h>

void far2cel(void) {
	// (a) Prompt user to enter value.
    printf("What is the temperature in Fahrenheit degrees? ");

	// (b) Declare local variable to hold input value.
    double F;
    double C;

	// (c) Read input value.
    scanf("%lf", &F);

	// (d) Compute output value.
    C = (5.000 / 9.000) * ( F - 32.000);

	// (e) Display the result.
    printf("%.3f Fahrenheit is the same as %.3f Celsius.\n", F, C);
    
}


int main(void) {
	far2cel();
	return 0;
}