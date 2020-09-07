#include "helpers.h"

void draw_border() {
    int w, h;
    get_screen_size(&w, &h);
    draw_line(((w-1)/3)+1, 0, ((w-1)/3)+1, 4, '|');
	draw_line((((w-1)/3)*2)+2, 0, (((w-1)/3)*2)+2, 4, '|');
    draw_line (0, 0, w-1, 0, '-');
    draw_line (0, 2, w-1, 2, '-');
}

void draw_intersections() {
    int w, h;
	get_screen_size(&w, &h);

	draw_char(0,0,'+');  //r1,c1
	draw_char(0,2,'+'); //r2,c1
	draw_char(0,4,'+'); //r3,c1
	draw_char(0,h-3,'+'); //rb, c1
	draw_char(((w-1)/3)+1, 0, '+'); //r1, c2
	draw_char(((w-1)/3)+1, 2, '+'); //r2, c2
	draw_char(((w-1)/3)+1,4,'+'); //r3, c2
	draw_char((((w-1)/3)*2)+2, 0, '+'); //r1, c3
	draw_char((((w-1)/3)*2)+2, 2, '+'); //r2, c3
	draw_char((((w-1)/3)*2)+2,4,'+'); //r3, c3
	draw_char(w-1,0,'+'); //r1, c4
	draw_char(w-1,2,'+'); //r2, c4
	draw_char(w-1,4,'+'); //r3, c4
	draw_char(w-1,h-3,'+'); //rb, c4
}

/**
 *  Draws a border around the terminal window using '*' symbols.
 */
void draw_display() {
    int w, h;
    get_screen_size(&w, &h);
    draw_rect(0, 4, w-1, h-3, '-'); // bounding box
    draw_border();
    draw_line(0, 0, 0, h-3, '|'); //left border
    draw_line(w - 1, 0, w - 1, h-3, '|'); //right border
    draw_intersections();
}
