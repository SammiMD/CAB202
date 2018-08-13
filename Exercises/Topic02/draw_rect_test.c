#include <stdlib.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>

//  (a) Declare four global integer variables, as follows:
//      l - the horizontal location of the left edge of the rectangle
int l, t, cols, rows;
//      t - the vertical location of the top edge of the rectangle 
//      cols - the width of the rectangle.
//      rows - the height of the rectangle.

//  (b) Declare a global variable of type char called symbol.
//      This is the character that is to be used to render the rectangle.
char symbol;

void draw_rect(void) {
    //  (c) Insert code to draw the outline of the rectangle defined by the global variables.
    //      If either of the width or height is less than or equal to zero,
    //      the function must not draw anything.
    //int left = 5, top = 2, right = w-1, bottom = h-1-1;
    if(cols<=0)
    return;
    if(rows<=0)
    return;
    int w = screen_width();
    int h = screen_height();
    int top=h-t;
    int left = w-l;
    int right=left+cols-1;
    int bottom=top-rows+1;
    
    
    draw_line(left, top, right, top, symbol);
    draw_line(right, top, right, bottom, symbol);
    draw_line(right, bottom, left, bottom, symbol);
    draw_line(left, bottom, left, top, symbol);
    
}

int main(void) {
    setup_screen();

    // draw a box.
    l = 23;
    t = 8;
    cols = 25;
    rows = 4;
    symbol = '@';
    draw_rect();
    show_screen();
/*
    // draw a box.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = 1 + rand() % (screen_width() - l - 1);
    rows = 1 + rand() % (screen_height() - t - 1);
    symbol = '&';
    draw_rect();
    show_screen();

    // draw a box with zero width.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = 0;
    rows = 1 + rand() % (screen_height() - t - 1);
    symbol = '*';
    draw_rect();
    show_screen();

    // draw a box.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = 1 + rand() % (screen_width() - l - 1);
    rows = 1 + rand() % (screen_height() - t - 1);
    symbol = '#';
    draw_rect();
    show_screen();

    // draw a box with negative width.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = -rand() % screen_width();
    rows = 1 + rand() % (screen_height() - t - 1);
    symbol = '!';
    draw_rect();
    show_screen();

    // draw a box.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = 1 + rand() % (screen_width() - l - 1);
    rows = 1 + rand() % (screen_height() - t - 1);
    symbol = '+';
    draw_rect();
    show_screen();

    // draw a box with zero height.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = 1 + rand() % (screen_width() - l - 1);
    rows = 0;
    symbol = 'a';
    draw_rect();
    show_screen();

    // draw a box.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = 1 + rand() % (screen_width() - l - 1);
    rows = 1 + rand() % (screen_height() - t - 1);
    symbol = 'b';
    draw_rect();
    show_screen();

    // draw a box with negative width.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = -rand() % screen_width();
    rows = 1 + rand() % (screen_height() - t - 1);
    symbol = 'c';
    draw_rect();
    show_screen();

    // draw a box.
    l = rand() % screen_width() / 2;
    t = rand() % screen_height() / 2;
    cols = 1 + rand() % (screen_width() - l - 1);
    rows = 1 + rand() % (screen_height() - t - 1);
    symbol = 'd';
    draw_rect();
    show_screen();
*/
    timer_pause(5000);
    cleanup_screen();
    return 0;
}
