#include <stdlib.h>
#include <cab202_graphics.h>

//  (a) Begin the definition a function called fill_rect that returns nothing, 
//      but requires the following arguments:
//      x1 - an integer that specifies the absolute horizontal location 
//          of the left hand side of the rectangle.
//      y1 - an integer that specifies the absolute vertical location 
//          of the upper edge of the rectangle.
//      x2 - an integer that specifies the absolute horizontal location 
//          of the right hand side of the rectangle.
//      y2 - an integer that specifies the absolute vertical location 
//          of the lower edge of the rectangle.
//      c = a character value that will be used to draw the
//          display the rectangle.

void fill_rect(int l, int t, int r, int b, char c) {

    //  (b) Test to see if either the width or height of the rectangle is less 
    //      than 1. If so, the function should return immediately without drawing anything.
    if((r-l)<0)
    return;

    if((b-t)<0)
    return;

    
    //  (c) Use statements of your own choice to fill the region defined by the
    //      first four arguments with the designated character value.
    //      There a many ways to achieve this, but any one of the following algorithms
    //      should suffice.
    
    // Option 1: Nested for loop; draw chars.
    //           FOR x = left to right
    //              FOR y = top to bottom
    //                 Draw char at (x,y)

    

    //
    // Option 2: Single loop; vertical lines.
    //           FOR x = left to right
    //              Draw line from (x,top) to (x,bottom)

        for (int x = l; x>=l && x<=r; x++)
    {
        draw_line(x, t, x, b, c);
    }
    

    // Option 3: Single loop; horizontal lines.
    //           FOR y = top to bottom
    //              Draw line from (left,y) to (right,y)

}


int main( void ) {
	int l, t, r, b;
	char c;

	printf( "Please enter the horizontal location of the left edge of the rectangle: " );
	scanf( "%d", &l );

	printf( "Please enter the vertical location of the top edge of the rectangle: " );
	scanf( "%d", &t );

	printf( "Please enter the horizontal location of the right edge of the rectangle: " );
	scanf( "%d", &r );

	printf( "Please enter the vertical location of the bottom edge of the rectangle: " );
	scanf( "%d", &b );

	printf( "Please enter the character used to draw the rectangle? " );
	scanf( " %c", &c );

	setup_screen();
	fill_rect( l, t, r, b, c );
	show_screen();
	wait_char();

	return 0;
}
