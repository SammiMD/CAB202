
#include <stdio.h>

int main(void) {
	printf("Please enter the temperature in Fahrenheit degrees: ");

	double fahr;
	scanf("%lf", &fahr);

	double celsius = (fahr - 32) * 5 / 9;
	printf("%f Fahrenheit is equivalent to %f Celsius.\n", fahr, celsius);

	return 0;
}