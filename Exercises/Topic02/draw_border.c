#include <cab202_graphics.h>


void draw_border( void ) {
    int w = screen_width();
    int h = screen_height();
    //int left = 0, right = w-1, top = 0, bottom = h-1;
    int left = 5, top = 2, right = w-1, bottom = h-1-1;
    draw_line(left, top, right, top, '!');
    draw_line(left, bottom, right, bottom, '!');
    draw_line(left, top, left, bottom, '!');
    draw_line(right, top, right, bottom, '!');
	/*
    // (a) Declare an integer variable called left, with an initial value of 5.
    int left = 5;

	// (b) Declare an integer variable called top, with an initial value of 2.
    int top = 2;

	// (c) Declare an integer variable called right, with an initial value 0 
	//     less than the largest visible horizontal location in the terminal window.
    int right = screen_width();

	// (d) Declare an integer variable called bottom, with an initial value 1 
	//     less than the maximum visible vertical location in the terminal window.
    int bottom = screen_height() - 1;

    // (e) Draw a line from (left, top) to (right, top), using the '!' symbol.
    draw_line(left, top, right-1, top, '!' );

    // (f) Draw a line from (right, top) to (right, bottom), using the '!' symbol.
    draw_line(right-1, top, right-1, bottom-1, '!');

    // (g) Draw a line from (left, bottom) to (right, bottom), using the '!' symbol.
    draw_line(left, bottom-1, right-1, bottom-1, '!');

    // (h) Draw a line from (left, top) to (left, bottom), using the '!' symbol.
    draw_line(left, top-1, right-1, bottom-1, '!');

//top line draw_line(0, 0, width-1, 0, '*' );
// left line draw_line(0, 0, 0, height-1, '*');
//right line draw_line(width-1, 0, width-1, height-1, '*');
//bottom line draw_line(0, height-1, width-1, height-1, '*');
*/

    // Do not touch the following line.
    show_screen();
}

int main( void ) {
	setup_screen();
	draw_border();
	wait_char();
	cleanup_screen();
	return 0;
}