#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"

// Global variables
bool game_over = false;
int delay = 10;
int timeout = INT_MAX;
double start_time;
#define SN "n5377544"
int heading, grams, battery, num_d, num_s, num_l;
int minutes, seconds;

//Robo State
#define ROBO_SPEED 0.2
#define ROBO_WIDTH 9
#define ROBO_HEIGHT 9
double rx, ry, rdx, rdy;

//Dust state
#define MAX_DUST 1000
#define DUST_HEIGHT 1
#define DUST_WIDTH 1
double dsx[MAX_DUST], dsy[MAX_DUST];

//Slime state
#define MAX_SLIME 1000
#define SLIME_HEIGHT 1
#define SLIME_WIDTH 1
double sx[MAX_SLIME], sy[MAX_SLIME];

//Litter state
#define MAX_LITTER 1000
#define LITTER_HEIGHT 1
#define LITTER_WIDTH 1
double lx[MAX_LITTER], ly[MAX_LITTER];

//element images
#define robo_img "    @    \n" "  @xxx@  \n" " @xxxxx@ \n" "@xxxxxxx@\n" "@xxxxxxx@\n" "@xxxxxxx@\n" " @xxxxx@ \n" "  @xxx@  \n" "    @    \n"
#define dock_img "#########\n" "#       #\n" "#########\n"
#define dust_img "."
#define slime_img "  ~  \n" " ~~~ \n" "~~~~~\n" " ~~~ \n" "  ~  \n"
#define litter_img "     &     \n" "    &&&    \n" "   &&&&&   \n" "  &&&&&&&  \n" " &&&&&&&&& \n" "&&&&&&&&&&&\n"

bool paused = false;

bool collided(
    double x0, double y0, int w0, int h0,
    double x1, double y1, int w1, int h1
);

 void HUD_calcs() {
    int secs = get_current_time() - start_time;
    minutes = abs(secs / 60);
    seconds = secs - (minutes*60);
 }

void draw_HUD() {
	int w, h;
	get_screen_size(&w, &h);

	//rectangles to make horizontal lines
	draw_rect(0, 0, w, 4, '-');
	draw_rect(0, 0, w, h-4, '-');
	draw_rect(0, 2, w, 2, '-');

    //vertical lines
	draw_line(0,0,0,h-4,'|'); //left line
	draw_line(w-1,0,w-1,h-4,'|'); //right line
	draw_line(((w-1)/3)+1, 0, ((w-1)/3)+1, 4, '|');
	draw_line((((w-1)/3)*2)+2, 0, (((w-1)/3)*2)+2, 4, '|');

    //intersection characters
	draw_char(0,0,'+');  //r1,c1
	draw_char(0,2,'+'); //r2,c1
	draw_char(0,4,'+'); //r3,c1
	draw_char(0,h-4,'+'); //rb, c1
	draw_char(((w-1)/3)+1, 0, '+'); //r1, c2
	draw_char(((w-1)/3)+1, 2, '+'); //r2, c2
	draw_char(((w-1)/3)+1,4,'+'); //r3, c2
	draw_char((((w-1)/3)*2)+2, 0, '+'); //r1, c3
	draw_char((((w-1)/3)*2)+2, 2, '+'); //r2, c3
	draw_char((((w-1)/3)*2)+2,4,'+'); //r3, c3
	draw_char(w-1,0,'+'); //r1, c4
	draw_char(w-1,2,'+'); //r2, c4
	draw_char(w-1,4,'+'); //r3, c4
	draw_char(w-1,h-4,'+'); //rb, c4

    int gap_size = (w/3)-5;

	draw_formatted((((w-1)/3) - gap_size)+(gap_size/2)-4, 1, "%s", SN);
    draw_formatted((((w-1)/3) - gap_size)+(gap_size/2)-7, 3, "Elapsed: %02d:%02d" , minutes, seconds);
	draw_formatted(((((w-1)/3)*2) - gap_size)+(gap_size/2)-4, 1, "Heading: %d", heading);
	draw_formatted(((((w-1)/3)*2) - gap_size)+(gap_size/2)-7, 3, "Load weight (g): %d", grams);
	draw_formatted(((((w-1)/3)*3) - gap_size)+(gap_size/2)-5, 1, "Battery: %d", battery);
	draw_formatted(((((w-1)/3)*3) - gap_size)+(gap_size/2)-11, 3, "Litter (d,s,l): (%d,%d,%d)", num_d, num_s, num_l);
}

/**
 *  Draw the robot vacuum at the centre of the allowable screen
 */
void draw_robo() {
    draw_formatted_lines(rx, ry, robo_img);
}

void draw_dock() {
    int w, h;
	get_screen_size(&w, &h);
    draw_formatted_lines(w/2,5, dock_img);
}

/* void draw_zombie(int i) {
    int left = round(zx[i]) - ZOMBIE_WIDTH / 2;
    int right = left + ZOMBIE_WIDTH - 1;

    int top = round(zy[i]) - ZOMBIE_HEIGHT / 2;
    int bottom = top + ZOMBIE_HEIGHT - 1;

    draw_line(left, top, right, top, 'Z');
    draw_line(right, top, left, bottom, 'Z');
    draw_line(left, bottom, right, bottom, 'Z');
} */

/**
 *  Draws all zombies.
 */
/* void draw_zombies() {
    for (int i = 0; i < num_zombies; i++) {
        draw_zombie(i);
    }
} */

/**
 *  Draw all objects in the simulation.
 */
void draw_all() {
    clear_screen();
    draw_HUD();
    draw_robo();
    draw_dock();
    //draw_zombies();
    show_screen();
}

/**
 *  Set up a single zombie at a random location.
 *
 *  Parameters:
 *      i: An integer satisfying 0 <= i < num_zombies which indicates the 
 *          object to be placed.
 *
 *  Notes:
 *      This function assigns random values to zx[i] and zy[i] so that the 
 *  object is guaranteed to be wholly contained within the border. A random
 *  velocity vector is set up in zdx[i] and zdy[i]. The zombie will take 
 *  approximately (1.0 / ZOMBIE_SPEED) time slices to travel one screen unit in
 *  the direction specified by the velocity vector.
 */
/* void setup_zombie(int i) {
    int w, h;
    get_screen_size(&w, &h);

    zx[i] = 1 + ZOMBIE_WIDTH / 2  + rand() % (w - 1 - ZOMBIE_WIDTH);
    zy[i] = 1 + ZOMBIE_HEIGHT / 2 + rand() % (h - 1 - ZOMBIE_HEIGHT);

    double zombie_dir = rand() * 2 * M_PI / RAND_MAX;
    const double step = ZOMBIE_SPEED;

    zdx[i] = step * cos(zombie_dir);
    zdy[i] = step * sin(zombie_dir);
} */

void setup_robo() {
    int w, h;
    get_screen_size(&w, &h);
    rx = w/2;
	ry = (h-8)/2;

    double robo_dir = rand() * 2 * M_PI / RAND_MAX;
    const double step = ROBO_SPEED;

    rdx = step * cos(robo_dir);
    rdy= step * sin(robo_dir);
}

/**
 *  Sets up all zombies (0 <= i < num_zombies) so that they are all 
 *  wholly contained within the border, and none overlap with the hero.
 */
/* void setup_zombies() {
    num_zombies = get_int("How many zombies would you like?");

    if (num_zombies < 1 || num_zombies > MAX_ZOMBIES) {
        num_zombies = 1;
    }

    for (int i = 0; i < num_zombies; i++) {
        do {
            setup_zombie(i);
        }
        while (collided(hx, hy, HERO_WIDTH, HERO_HEIGHT, zx[i], zy[i], ZOMBIE_WIDTH, ZOMBIE_HEIGHT));
    }
} */

/**
 *  Set up all objects in the simulation.
 */
void setup(void) {
    start_time = get_current_time();
    //setup_zombies();
    setup_robo();
    draw_all();
}

/**
 *  Returns true iff and only if the supplied argument is a robovac navigation control.
 */
bool is_robo_ctrl(int ch) {
    return (ch == 'i' || ch == 'j' || ch == 'k' || ch == 'l');
}

/**
 *  Update the robovac location based on the supplied argument.
 *
 *  Parameters:
 *      ch: a character code which is expected to be one of [ijkl].
 *          'j' -> move left
 *          'k' -> move down
 *          'l' -> move right
 *          'i' -> move up
 *
 *  Notes:
 *      If the designated move would cause the bounding box of the robovac
 *  icon to overlap the border then the move is ignored. 
 */
void move_robo(int ch) {
    if (ch == 'j' && rx - ROBO_WIDTH / 2 > 1) {
        rx--;
    }
    else if (ch == 'l' && rx + ROBO_WIDTH / 2 < screen_width() - 2) {
        rx++;
    }
    else if (ch == 'k' && ry + ROBO_HEIGHT / 2 < screen_height() - 2) {
        ry++;
    }
    else if (ch == 'i' && ry - ROBO_HEIGHT / 2 > 1) {
        ry--;
    }
}

/**
 *  Update the location of a zombie based on its stored velocity.
 *
 *  Parameters:
 *      i: the ID of a zombie, satisfying 0 <= i < num_zombies.
 *
 *  Notes:
 *      If the designated move would cause the bounding box of the zombie
 *  icon to overlap the border then one or both of the velocity components
 *  is negated, as appropriate to cause the zombie to reflect off the wall.
 *  When that happens the zombie does not move.
 */
void update_robo() {
    // Assume that we have not already collided with the wall.
    // Predict the next screen position of the robovac.
    int new_x = round(rx + rdx) - ROBO_WIDTH / 2;
    int new_y = round(ry + rdy) - ROBO_HEIGHT / 2;

    bool bounced = false;

    if (new_x == 0 || new_x + ROBO_WIDTH == screen_width()) {
        // horizontal bounce
        rdx = -rdx;
        bounced = true;
    }

    if (new_y == 4 || new_y + ROBO_HEIGHT == screen_height()-7) {
        // horizontal bounce
        rdy = -rdy;
        bounced = true;
    }

    if (!bounced) {
        rx += rdx;
        ry += rdy;
    }
}

/**
 *  Returns true if and only if two supplied bounding boxes overlap.
 *
 *  Parameters:
 *      (x0,y0) - real-valued coordinates of the centre of the first bounding box.
 *      (w0,h0) - integer-valued dimensions of the first bounding box.
 *      (x1,y1) - real-valued coordinates of the centre of the second bounding box.
 *      (w1,h1) - integer-valued dimensions of the second bounding box.
 */
bool collided(
    double x0, double y0, int w0, int h0,
    double x1, double y1, int w1, int h1
) {
    int left0 = round(x0) - w0 / 2, right0 = left0 + w0 - 1;
    int left1 = round(x1) - w1 / 2, right1 = left1 + w1 - 1;

    int top0 = round(y0) - h0 / 2, bottom0 = top0 + h0 - 1;
    int top1 = round(y1) - h1 / 2, bottom1 = top1 + h1 - 1;

    if (right1 < left0 || right0 < left1) return false;
    if (bottom1 < top0 || bottom0 < top1) return false;

    return true;
}

/**
 *  Resets the simulation.
 */
void reset() {
    setup();
}

/**
 *  Obtain a time-out value from standard input and store it for future use.
 */
void do_timeout() {
    timeout = get_int("How many seconds shall the program run?");
}

/**
 *  Returns true iff the simulation has been running for longer than the 
 *  current designated time-out value.
 */
bool timed_out() {
    return get_current_time() - start_time >= timeout;
}

/**
 *  Action taken when a zombie has collided with the hero.
 *  Displays a message, waits for a response from standard input
 *  (or simulation time-out), then resets the simulation.
 */

/* void do_collided() {
    clear_screen();

    const char *message[] = {
        "You're zombie food!",
        "Press any key to restart..."
    };

    const int rows = 2;

    for (int i = 0; i < rows; i++) {
        int len = strlen(message[i]);
        int x = (screen_width() - len) / 2;
        int y = i + (screen_height() - rows) / 2;
        draw_formatted(x, y, message[i]);
    }

    show_screen();

    timer_pause(1000);
    while (get_char() > 0) {}
    while (!timed_out() && wait_char() <= 0) {}

    reset();
} */

/**
 *  Tests to see if any zombie has collided with the hero. If so,
 *  do_collided is called to restart the simulation.
 */
/* void check_collision() {
    for (int i = 0; i < num_zombies; i++) {
        if (collided(hx, hy, HERO_WIDTH, HERO_HEIGHT, zx[i], zy[i], ZOMBIE_WIDTH, ZOMBIE_HEIGHT)) {
            do_collided();
            return;
        }
    }
} */

/**
 *  A dialogue which allows the position and velocity of a designated zombie to 
 *  be manually overridden.
 */
void do_robo_cheat() {
    int x = get_int("x co-ordinate?");
    int y = get_int("y co-ordinate?");
    int angle = get_int("heading in degrees?");

    rx = x;
    ry = y;
    rdx = ROBO_SPEED * cos(angle * M_PI / 180);
    rdy = ROBO_SPEED * sin(angle * M_PI / 180);
}

/**
 *  Allows user to change payload weight of robovac
 */
void do_weight_cheat() {
    grams = get_int("enter payload weight in grams");
}

/**
 *  Allows user to change the remaining battery percentage
 */
void do_battery_cheat() {
    battery = get_int("enter remaining battery in percentage");
}

/**
 *  Toggles robovac motion between paused and unpaused.
 */
void do_pause() {
    paused = !paused;
}

/**
 *  Dialogue which allows the loop delay to be manually overridden.
 */
void do_delay_cheat() {
    delay = get_int("New delay (milliseconds)?");

    if (delay < 0) delay = 10;
}

/**
 *  Dialog which displays a help screen.
 */
void do_help() {
    clear_screen();
    draw_formatted_lines(0, 0,
        "***************************************\n"
        "**                                   **\n"
        "** Robot Vacuum Cleaner Instructions **\n"
        "**                                   **\n"
        "***************************************\n"
        "\n"
        "j, k, l, i: Move Robot Vacuum left, down, right, up respectively\n"
        "b: Return the Robovac to charging dock\n"
        "d: Drop a piece of dust\n"
        "s: Drop a piece of slime\n"
        "t: Drop a piece of trash/litter\n"
        "m: change the millisecond loop delay\n"
        "o: time out for a user specified number of milliseconds\n"
        "v: Robovac movement cheat - override the position and heading of the Robovac\n"
        "w: Robovac weight cheat - override the current payload weight of the Robovac\n"
        "y: Robovac battery cheat - override the current remaining battery percentage of the Robovac\n"
        "p: pause or resume Robovac movement\n"  
        "r: restart\n"
        "q: quit\n"
        "?: display this help screen\n\n\n"
        "Press a key to return to the game."
    );
    show_screen();

    while (get_char() > 0) {}
    while (!timed_out() && wait_char() <= 0) {}
}

/**
 *  Jump table which chooses the action (if any) which corresponds to a 
 *  supplied command character.
 *
 *  Parameters:
 *      ch: a command character. Currently recognised comands are listed in 
 *          the do_help function.
 */
void do_operation(int ch) {
    if (ch == 'r') {
        reset();
    }
    else if (is_robo_ctrl(ch)) {
        move_robo(ch);
    }
    else if (ch == 'o') {
        do_timeout();
    }
    else if (ch == 'v') {
        do_robo_cheat();
    }
    else if (ch == 'h') {
        //do_hero_cheat();
    }
    else if (ch == 'w') {
        do_weight_cheat();
    }
    else if (ch == 'y') {
        do_battery_cheat();
    }
    else if (ch == 'p') {
        do_pause();
    }
    else if (ch == 'm') {
        do_delay_cheat();
    }
    else if (ch == '?') {
        do_help();
    }
}

/**
 *  The loop function: called once per time slice to run the simulation.
 */
void loop() {
    int ch = get_char();

    if (ch == 'q') {
        game_over = true;
        return;
    }
    else if (ch >= ' ') {
        do_operation(ch);
    }
    else if (!paused) {
        update_robo();
    }
    HUD_calcs();

    //check_collision();

    draw_all();
}

/**
 *  Program start and event loop.
 */
int main(void) {

    // srand(9139532); // For initial testing 
    srand(get_current_time()); // For production
    battery = 100;
    paused = true;
    heading = 90;
    setup_screen();
    setup();

    while (!game_over && !timed_out()) {
        loop();
        timer_pause(delay);
    }

    return 0;
}
