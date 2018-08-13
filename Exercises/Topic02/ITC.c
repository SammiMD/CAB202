#include <stdio.h>
#include <math.h>
int main(void) { 
    printf("Please enter taxable income in whole dollars: "); 

    int income; 
    scanf("%d", &income); 

    double tax; 

    if (income <= 18200) { 
        tax = 0; 
        } 
        else if (income <= 37000) { 
            tax = 0.19 * (income - 18200); 
        } 
        else if (income <= 87000) { 
            tax = 3572 + 0.325 * (income - 37000); 
        } 
        else if (income <= 180000) { 
            tax = 19822 + 0.37 * (income - 87000); 
        } 
        else { 
            tax = 54232 + 0.45 * (income - 180000); 
        } 
        tax = round(tax * 100) / 100; 

        printf("Tax payable on $%0.2f is $%0.2f\n", (double)income, tax); 

        return 0; 
        
}