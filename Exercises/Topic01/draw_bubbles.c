#include "cab202_graphics.h"

void draw_bubbles(void){
	// (a) Draw a bubble at (33,16)
    draw_char(62, 12, 'O');

	// (b) Draw a bubble at (48,6)
    draw_char(6, 8, 'O');
	// (c) Draw a bubble at (41,14)
    draw_char(53, 4, 'O');
	// (d) Draw a bubble at (14,3)
    draw_char(11, 0, 'O');
	// Keep the following line without change.
	show_screen();
}

int main( void ) {
	setup_screen();

	draw_bubbles();

	draw_string( 0, screen_height() - 1, "Press any key to finish..." );
	wait_char();
	cleanup_screen();
	return 0;
}