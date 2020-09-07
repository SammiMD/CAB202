#include <math.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <cab202_graphics.h>
#include <cab202_timers.h>
#include "helpers.h"
#include "setup_graphics.h"

// Global variables
#define SN "n5377544"
#define MAX_TRASH 65
bool game_over = false;
bool RTB = false;
bool pump = true;
bool docked = false;
int delay = 10;
int timeout = INT_MAX;
double start_time, docked_time, inc_bat, rdx0, rdy0;
int num_d, count_d, num_s, count_s, num_l, count_l, battery, heading, grams, minutes, seconds, secs, secs2;

//Robo State
#define ROBO_SPEED 0.2
#define ROBO_WIDTH 9
#define ROBO_HEIGHT 9
#define MAX_LOAD 65
double rx, ry, rdx, rdy;

//Dock State
#define DOCK_WIDTH 9
#define DOCK_HEIGHT 3
double dkx, dky;

//Dust state
#define MAX_DUST 1000
#define DUST_HEIGHT 1
#define DUST_WIDTH 1
#define DUST_MASS 1
double dsx[MAX_DUST], dsy[MAX_DUST];

//Slime state
#define MAX_SLIME 10
#define SLIME_HEIGHT 5
#define SLIME_WIDTH 5
#define SLIME_MASS 5
double sx[MAX_SLIME], sy[MAX_SLIME];

//Litter state
#define MAX_LITTER 5
#define LITTER_HEIGHT 6
#define LITTER_WIDTH 11
#define LITTER_MASS 20
double lx[MAX_LITTER], ly[MAX_LITTER];

//element images
char *robo_img =
    "    @    "
    "  @xxx@  "
    " @xxxxx@ "
    "@xxxxxxx@"
    "@xxxxxxx@"
    "@xxxxxxx@"
    " @xxxxx@ "
    "  @xxx@  "
    "    @    ";

char *dust_img =
    ".";

char *dock_img =
    "#########"
    "#|||||||#"
    "#########";

char *slime_img =
    "  ~  "
    " ~~~ "
    "~~~~~"
    " ~~~ "
    "  ~  ";

char *litter_img =
    "     &     "
    "    &&&    "
    "   &&&&&   "
    "  &&&&&&&  "
    " &&&&&&&&& "
    "&&&&&&&&&&&";

bool paused = false;

bool is_opaque(int x, int y, int x0, int y0, int w0, int h0, char px0[])
{
    return x >= x0 && x < x0 + w0 && y >= y0 && y < y0 + h0 && px0[(x - x0) + (y - y0) * w0] != ' ';
}

bool pixel_collision(
    double x0, double y0, int w0, int h0, char px0[],
    double x1, double y1, int w1, int h1, char px1[])
{
    for (int j = 0; j < h0; j++)
    {
        for (int i = 0; i < w0; i++)
        {
            int x = x0 + i;
            int y = y0 + j;
            if (is_opaque(x, y, x0, y0, w0, h0, px0) && is_opaque(x, y, x1, y1, w1, h1, px1))
            {
                return true;
            }
        }
    }
    return false;
}

void get_vals() {
    num_d = 0;
    num_d = get_int("How many dust motes would you like?");
    if (num_d > MAX_DUST)
    {
        num_d = MAX_DUST;
    }
    num_s = 0;
    num_s = get_int("How many slime piles would you like?");
    if (num_s > MAX_SLIME)
    {
        num_s = MAX_SLIME;
    }
    num_l = 0;
    num_l = get_int("How many litter piles would you like?");
    if (num_l > MAX_LITTER)
    {
        num_l = MAX_LITTER;
    }
}

/**
  * Perform calculations for HUD
  **/
void HUD_calcs()
{
    int s1 = secs;
    secs = get_current_time() - start_time;
    int s2 = secs;
    minutes = abs(secs / 60);
    seconds = secs - (minutes * 60);
    if ((s1 < s2) && battery != 0 && docked == false)
    {
        battery--;
    }
}

void draw_status()
{
    if (docked)
    {
        status_update("Docked");
    }
}

/**
  * Populate HUD with game variables
**/

void populate_HUD()
{
    int w, h;
    get_screen_size(&w, &h);
    int gap_size = (w / 3) - 5;

    draw_formatted((((w - 1) / 3) - gap_size) + (gap_size / 2) - 4, 1, "%s", SN);
    draw_formatted((((w - 1) / 3) - gap_size) + (gap_size / 2) - 7, 3, "Elapsed: %02d:%02d", minutes, seconds);
    draw_formatted(((((w - 1) / 3) * 2) - gap_size) + (gap_size / 2) - 4, 1, "Heading: %d", heading);
    draw_formatted(((((w - 1) / 3) * 2) - gap_size) + (gap_size / 2) - 7, 3, "Load weight (g): %d", grams);
    draw_formatted(((((w - 1) / 3) * 3) - gap_size) + (gap_size / 2) - 5, 1, "Battery: %d %%", battery);
    draw_formatted(((((w - 1) / 3) * 3) - gap_size) + (gap_size / 2) - 11, 3, "Litter (d,s,l): (%d,%d,%d)", count_d, count_s, count_l);
}

//Collect Dust
void collect_dust()
{
    int w, h;
    get_screen_size(&w, &h);
    //check if colliding with any dust
    for (int i = 0; i < num_d; i++)
    {
        if (pixel_collision(rx, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dsx[i], dsy[i], DUST_WIDTH, DUST_HEIGHT, dust_img))
        {
            dsx[i] = w;
            dsy[i] = 0;
            count_d--;
            grams += DUST_MASS;
            show_screen();
        }
    }
}

void collect_slime()
{
    int w, h;
    get_screen_size(&w, &h);
    for (int j = 0; j < num_s; j++)
    {
        if (pixel_collision(rx, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img, sx[j], sy[j], SLIME_WIDTH, SLIME_HEIGHT, slime_img))
        {
            sx[j] = w;
            sy[j] = 0;
            count_s--;
            grams += SLIME_MASS;
            show_screen();
        }
    }
}

void collect_litter()
{
    int w, h;
    get_screen_size(&w, &h);
    for (int k = 0; k < num_l; k++)
    {
        if (pixel_collision(rx, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img, lx[k], ly[k], LITTER_WIDTH, LITTER_HEIGHT, litter_img))
        {
            lx[k] = w;
            ly[k] = 0;
            count_l--;
            grams += LITTER_MASS;
            show_screen();
        }
    }
}


/** Trash Collection
 * Calls three functions that check for robovac and trash pixel collision.
 * Will only run when the payload is below 45 grams and the robovac is not in Return To Base (pump off) mode
 */
void collect_trash()
{
    if (pump == true && grams <= 45)
    {
        int w, h;
        get_screen_size(&w, &h);
        //check if colliding with any dust
        collect_dust();
        collect_slime();
        collect_litter();
    }
}

/**
 *  Draw dock centred on dkx, dky
 *
 */
void draw_dock()
{
    draw_pixels(dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img, true);
}

/**
 *  Draw robo centred on rx, ry
 *
 */
void draw_robo()
{
    draw_pixels(rx, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img, true);
}

/**
 *  Draw one dust mote, indicated by dust ID i. The object is centred on 
 *  (dsx[i],dsy[i]).
 *
 *  Parameters:
 *      i: An integer which satisfiess 0 <= i < num_d.
 */
void draw_dust(int i)
{
    draw_pixels(dsx[i], dsy[i], DUST_WIDTH, DUST_HEIGHT, dust_img, true);
}

/**
 *  Draws all dust motes.
 */
void draw_dust_motes()
{
    for (int i = 0; i < num_d; i++)
    {
        draw_dust(i);
    }
}

/**
 *  Draw one slime pile, indicated by slime ID i. The object is centred on 
 *  (sx[i],sy[i]).
 *
 *  Parameters:
 *      i: An integer which satisfiess 0 <= i < num_s.
 */
void draw_slime(int i)
{
    draw_pixels(sx[i], sy[i], SLIME_WIDTH, SLIME_HEIGHT, slime_img, true);
}

/**
 *  Draws all slime piles.
 */
void draw_slime_piles()
{
    for (int i = 0; i < num_s; i++)
    {
        draw_slime(i);
    }
}

/**
 *  Draw one litter pile, indicated by litter ID i. The object is centred on 
 *  (lx[i],ly[i]).
 *
 *  Parameters:
 *      i: An integer which satisfiess 0 <= i < num_l.
 */
void draw_litter(int i)
{
    draw_pixels(lx[i], ly[i], LITTER_WIDTH, LITTER_HEIGHT, litter_img, true);
}

/**
 *  Draws all slime piles.
 */
void draw_litter_piles()
{
    for (int i = 0; i < num_l; i++)
    {
        draw_litter(i);
    }
}

/**
 *  Draw all objects in the simulation.
 */
void draw_all()
{
    clear_screen();
    draw_display();
    populate_HUD();
    draw_dock();
    draw_dust_motes();
    draw_slime_piles();
    draw_litter_piles();
    draw_robo();
    draw_status();
    show_screen();
}

/**
 *  Set up dock at the centre of the usable screen directly below the HUD.
 */
void setup_dock()
{
    int w, h;
    get_screen_size(&w, &h);
    dkx = w / 2;
    dky = 5;
}

/**
 *  Set up robo at the centre of the usable screen.
 *
 *  Notes:
 *  Initial velocity vector (rdx, rdy) is set to 0.0 and 0.2 respectively.
 */
void setup_robo()
{
    int w, h;
    get_screen_size(&w, &h);
    rx = w / 2;
    ry = (h - 6) / 2;

    rdx = 0.0;
    rdy = ROBO_SPEED;
}

/**
 *  Set up a single dust mote at a random location.
 *
 *  Parameters:
 *      i: An integer satisfying 0 <= i < num_d which indicates the 
 *          object to be placed.
 *
 *  Notes:
 *      This function assigns random values to dsx[i] and dsy[i] so that the 
 *  object is guaranteed to be wholly contained within the border. 
 */
void setup_dust(int i)
{
    int w, h;
    get_screen_size(&w, &h);

    dsx[i] = 1 + DUST_WIDTH + rand() % (w - 2 - DUST_WIDTH);
    dsy[i] = 4 + DUST_HEIGHT + rand() % (h - 8 - DUST_HEIGHT);
}

/**
 *  Sets up all dust motes (0 <= i < num_d) so that they are all 
 *  wholly contained within the border, and none overlap with the robovac.
 */
void setup_dust_motes()
{
    count_d = num_d;
    for (int i = 0; i < num_d; i++)
    {
        setup_dust(i);

        bool dustcoll = false;
        while (!dustcoll)
        {
            if (i == 0)
            {
                break;
            }
            else
            {
                for (int j = 0; j < i; j++)
                {
                    if (pixel_collision(dsx[j], sy[j], DUST_WIDTH + 1, DUST_HEIGHT + 1, dust_img,
                                        dsx[i], dsy[i], DUST_WIDTH + 1, DUST_HEIGHT + 1, dust_img))
                    {
                        setup_dust(i);
                    }
                    else
                    {
                        dustcoll = true;
                    }
                }
            }
        }
        bool slimecoll = false;
        while (!slimecoll)
        {
            if (i == 0 || num_s == 0)
            {
                break;
            }
            else
            {
                for (int j = 0; j < num_s; j++)
                {
                    if (pixel_collision(sx[j], sy[j], SLIME_WIDTH + 1, SLIME_HEIGHT + 1, slime_img,
                                        dsx[i], dsy[i], DUST_WIDTH + 1, DUST_HEIGHT + 1, dust_img))
                    {
                        setup_dust(i);
                    }
                    else
                    {
                        slimecoll = true;
                    }
                }
            }
        }
        bool littercoll = false;
        while (!littercoll)
        {
            if (i == 0 || num_l == 0)
            {
                break;
            }
            else
            {
                for (int j = 0; j < num_l; j++)
                {
                    if (pixel_collision(lx[j], ly[j], LITTER_WIDTH + 1, LITTER_HEIGHT + 1, litter_img,
                                        dsx[i], dsy[i], DUST_WIDTH + 1, DUST_HEIGHT + 1, dust_img))
                    {
                        setup_dust(i);
                    }
                    else
                    {
                        littercoll = true;
                    }
                }
            }
        }

        bool collision = false;
        while (!collision)
        {
            if (pixel_collision(rx, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dsx[i], dsy[i], DUST_WIDTH, DUST_HEIGHT, dust_img) || pixel_collision(dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img, dsx[i], dsy[i], DUST_WIDTH, DUST_HEIGHT, dust_img))
            {
                setup_dust(i);
            }
            else
            {
                collision = true;
            }
        }
    }
}

/**
 *  Set up a single slime pile at a random location.
 *
 *  Parameters:
 *      i: An integer satisfying 0 <= i < num_s which indicates the 
 *          object to be placed.
 *
 *  Notes:
 *      This function assigns random values to sx[i] and sy[i] so that the 
 *  object is guaranteed to be wholly contained within the border. 
 */
void setup_slime(int i)
{
    int w, h;
    get_screen_size(&w, &h);

    sx[i] = 1 + SLIME_WIDTH / 2 + rand() % (w - 3 - SLIME_WIDTH);
    sy[i] = 5 + SLIME_HEIGHT / 2 + rand() % (h - 9 - SLIME_HEIGHT);
}

/**
 *  Sets up all dust motes (0 <= i < num_d) so that they are all 
 *  wholly contained within the border, and none overlap with the robovac.
 */
void setup_slime_piles()
{
    count_s = num_s;
    for (int i = 0; i < num_s; i++)
    {
        setup_slime(i);

        bool slimecoll = false;
        while (!slimecoll)
        {
            if (i == 0)
            {
                break;
            }
            else
            {
                for (int j = 0; j < i; j++)
                {
                    if (pixel_collision(sx[j], sy[j], SLIME_WIDTH + 1, SLIME_HEIGHT + 1, slime_img,
                                        sx[i], sy[i], SLIME_WIDTH + 1, SLIME_HEIGHT + 1, slime_img))
                    {
                        setup_slime(i);
                    }
                    else
                    {
                        slimecoll = true;
                    }
                }
            }
        }
        bool littercoll = false;
        while (!littercoll)
        {
            if (i == 0 || num_l == 0)
            {
                break;
            }
            else
            {
                for (int j = 0; j < num_l; j++)
                {
                    if (pixel_collision(lx[j], ly[j], LITTER_WIDTH + 1, LITTER_HEIGHT + 1, litter_img,
                                        sx[i], sy[i], SLIME_WIDTH + 1, SLIME_HEIGHT + 1, slime_img))
                    {
                        setup_slime(i);
                    }
                    else
                    {
                        littercoll = true;
                    }
                }
            }
        }
        bool collision = false;
        while (!collision)
        {
            if (pixel_collision(rx, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img,
                                sx[i], sy[i], SLIME_WIDTH, SLIME_HEIGHT, slime_img) ||
                pixel_collision(dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img,
                                sx[i], sy[i], SLIME_WIDTH, SLIME_HEIGHT, slime_img))
            {
                setup_slime(i);
            }
            else
            {
                collision = true;
            }
        }
    }
}

/**
 *  Set up a single dust mote at a random location.
 *
 *  Parameters:
 *      i: An integer satisfying 0 <= i < num_d which indicates the 
 *          object to be placed.
 *
 *  Notes:
 *      This function assigns random values to dsx[i] and dsy[i] so that the 
 *  object is guaranteed to be wholly contained within the border. 
 */
void setup_litter(int i)
{
    int w, h;
    get_screen_size(&w, &h);

    lx[i] = 1 + LITTER_WIDTH + rand() % (w - 12 - LITTER_WIDTH);
    ly[i] = 4 + LITTER_HEIGHT + rand() % (h - 12 - LITTER_HEIGHT);
}

/**
 *  Sets up all litter piles (0 <= i < num_l) so that they are all 
 *  wholly contained within the border, and none overlap with the robovac.
 */
void setup_litter_piles()
{
    count_l = num_l;
    for (int i = 0; i < num_l; i++)
    {
        setup_litter(i);
        bool littercoll = false;
        while (!littercoll)
        {
            if (i == 0)
            {
                break;
            }
            else
            {
                for (int j = 0; j < i; j++)
                {
                    if (pixel_collision(lx[j], ly[j], LITTER_WIDTH + 1, LITTER_HEIGHT + 1, litter_img,
                                        lx[i], ly[i], LITTER_WIDTH + 1, LITTER_HEIGHT + 1, litter_img))
                    {
                        setup_litter(i);
                    }
                    else
                    {
                        littercoll = true;
                    }
                }
            }
        }
        bool collision = false;
        while (!collision)
        {
            if (pixel_collision(rx, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img,
                                lx[i], ly[i], LITTER_WIDTH, LITTER_HEIGHT, litter_img) ||
                pixel_collision(dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img,
                                lx[i], ly[i], LITTER_WIDTH, LITTER_HEIGHT, litter_img))
            {
                setup_litter(i);
            }
            else
            {
                collision = true;
            }
        }
    }
}

/**
 *  Set up all objects in the simulation.
 */
void setup(void)
{

    start_time = get_current_time();
    battery = 100;
    grams = 0;
    paused = true;
    pump = true;
    RTB = false;
    docked = false;
    heading = 90;
    rdx = 0.0;
    rdy = 0.2;
    secs = 0.0;
    get_vals();
    setup_litter_piles();
    setup_slime_piles();
    setup_dust_motes();
    setup_dock();
    setup_robo();
    draw_all();
}

/**
 *  Returns true iff and only if the supplied argument is a hero navigation control.
 */
bool is_robo_ctrl(int ch)
{
    return (ch == 'j' || ch == 'k' || ch == 'l' || ch == 'i');
}

/**
 *  Update the robovac location based on the supplied argument.
 *
 *  Parameters:
 *      ch: a character code which is expected to be one of [asdw].
 *          'j' -> move left
 *          'k' -> move down
 *          'l' -> move right
 *          'i' -> move up
 *
 *  Notes:
 *      If the designated move would cause the bounding box of the robovac
 *  icon to overlap the border then the move is ignored. 
 */
void move_robo(int ch)
{
    if (ch == 'j' && rx - ROBO_WIDTH / 2 >= 2 - ROBO_WIDTH / 2)
    {
        if (pixel_collision(rx - 1, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img) == false)
        {
            rx--;
        }
    }
    else if (ch == 'l' && rx + ROBO_WIDTH / 2 < screen_width() - 2 - ROBO_WIDTH / 2)
    {
        if (pixel_collision(rx + 1, ry, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img) == false)
        {
            rx++;
        }
    }
    else if (ch == 'k' && ry + ROBO_HEIGHT / 2 < screen_height() - 4 - ROBO_HEIGHT / 2)
    {
        if (pixel_collision(rx, ry + 1, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img) == false)
        {
            ry++;
        }
    }
    else if (ch == 'i' && ry - ROBO_HEIGHT / 2 > 5 - ROBO_HEIGHT / 2)
    {
        if (pixel_collision(rx, ry - 1, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img) == false)
        {
            ry--;
        }
    }
}

/**
 *  Update the location of robo based on its stored velocity.
 *
 *  Notes:
 *      If the designated move would cause a pixel of the robo
 *  icon to overlap the border then one or both of the velocity components
 *  is negated, as appropriate to cause the robo to reflect off the wall.
 *  When that happens the robo does not move.
 */
void update_robo()
{
    // Assume that we have not already collided with the wall or dock.
    // Predict the next screen position of the robo.
    if (docked)
    {
        rdx = 0.0;
        rdy = 0.0;
    }
    double new_x = round(rx + rdx);
    double new_y = round(ry + rdy);

    double angle_change = ((rand() % 30) + 30);
    double new_angle;

    bool bounced = false;

    if (new_x <= 1 || new_x + ROBO_WIDTH == screen_width() || new_y < 6 || new_y + ROBO_HEIGHT > screen_height() - 3 || (pixel_collision(new_x, new_y - 1, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img)) || (pixel_collision(new_x - 1, new_y - 1, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img)))
    {

        timer_pause(delay);

        if (rand() % 2 == 0)
        {
            new_angle = angle_change * -1;
        }
        else
        {
            new_angle = angle_change * 1;
        }

        if (heading >= 180)
        {
            heading = heading - 180 + new_angle;
        }
        if (heading < 180)
        {
            heading = heading + 180 + new_angle;
        }
        if (heading >=360) {
            heading = heading - 360;
        }

        rdx = -1 * ROBO_SPEED * cos(heading * M_PI / 180);
        rdy = ROBO_SPEED * sin(heading * M_PI / 180);

        bounced = true;
    }

    if (!bounced)
    {
        rx += rdx;
        ry += rdy;
    }
}

/**
 *  Resets the simulation.
 */
void reset()
{
    setup();
}

/**
 *  Obtain a time-out value from standard input and store it for future use.
 */
void do_timeout()
{
    timeout = get_int("How many seconds shall the program run?");
}

/**
 *  Returns true iff the simulation has been running for longer than the 
 *  current designated time-out value.
 */
bool timed_out()
{
    return get_current_time() - start_time >= timeout;
}

/**
 * Run Game Over scenario, quit simulation with a quit screen. 
 */
void do_game_over()
{
    
    game_over = true;
    clear_screen();

    const char *message[] = {
        "Thanks for playing the Robot Vacuum Simulator",
        "Press any key to quit the application..."};

    const int rows = 2;

    for (int i = 0; i < rows; i++)
    {
        int len = strlen(message[i]);
        int x = (screen_width() - len) / 2;
        int y = i + (screen_height() - rows) / 2;
        draw_formatted(x, y, message[i]);
    }

    show_screen();
    while (get_char() > 0)
    {
    }
    while (!timed_out() && wait_char() <= 0)
    {
    }

    game_over = true;
    return;
}

void charge_battery()
{
    if (docked)
    {
        int s1 = abs(secs2);
        secs2 = get_current_time() - docked_time;
        int s2 = abs(secs2);
        if ((s1 < s2) && battery != 100 && secs2 < 3)
        {
            if (battery < 100)
            {
                battery = battery + inc_bat;
            }
            else if (battery > 100)
            {
                battery = 100;
            }
        }
        else if (secs2 >= 3)
        {
            battery = 100;
            rdx = rdx0;
            rdy = rdy0;
            docked = false;
            RTB = false;
            pump = true;
            grams = 0;
        }
    }
}

/**
 * Run Return To Base scenario.
 * Move robo directly to the charging station
 * Dock, charge battery to 100% over 3 seconds, and offload payload
 * Do not pick up any dust, slime, or litter while returning to base
 * Resume standard motion once battery is 100%
 */
void return_to_base()
{
    if (RTB == true)
    {
        pump = false;
        if (rx < dkx && ry <= 7)
        {
            heading = 180;
            rdx = -1 * ROBO_SPEED * cos(heading * M_PI / 180);
            rdy = ROBO_SPEED * sin(heading * M_PI / 180);
        }
        else if (rx > dkx && ry <= 7)
        {
            heading = 0;
            rdx = -1 * ROBO_SPEED * cos(heading * M_PI / 180);
            rdy = ROBO_SPEED * sin(heading * M_PI / 180);
        }
        else
        {
            double vx = dkx - rx;
            double vy = dky - ry;
            double angle = atan2(vy, vx);

            rdx = ROBO_SPEED * cos(angle);
            rdy = ROBO_SPEED * sin(angle);

            heading = atan2(rdy, rdx) * (180/M_PI) + 360 + 90;
            if (heading > 360) {
                heading = heading -360;
            }
        }

        double new_x = round(rx + rdx);
        double new_y = round(ry + rdy);

        if (((pixel_collision(new_x, new_y - 1, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img)) || (pixel_collision(new_x - 1, new_y - 1, ROBO_WIDTH, ROBO_HEIGHT, robo_img, dkx, dky, DOCK_WIDTH, DOCK_HEIGHT, dock_img))) && docked == false)
        {
            docked = true;
            docked_time = get_current_time();
            double docked_bat = battery;
            inc_bat = ((100 - docked_bat) / 3);
            secs2 = 0.0;
            rdx0 = rdx;
            rdy0 = rdy;
        }
    }
}

/** 
 * Run Dead Battery scenario.
 * Display Simulation Over message
 * Give user option of quitting or resetting simulation
 * 
 * NOTE: second character press option requires button to be pressed twice to be recognised.
 */
void dead_battery()
{
    status_update(" ");
    int w, h;
    get_screen_size(&w, &h);
    const char *message[] = {
        "*****************************************************",
        "**                SIMULATION OVER!                 **",
        "**          You have run out of battery!           **",
        "** Press q to quit or r to reset the simulation... **",
        "*****************************************************"};

    const int rows = 5;

    for (int i = 0; i < rows; i++)
    {
        int len = strlen(message[i]);
        int x = (w + 7 - len) / 2;
        int y = i + (h + 2 - rows) / 2;
        draw_formatted(x, y, message[i]);
    }
    show_screen();
    timer_pause(delay);
    int ch = getchar();
    if (ch == 'q')
    {
        do_game_over();
    }
    else if (ch == 'r')
    {
        reset();
    }
}

/**
 *  Tests to see if the battery percentage has dropped below 25%
 *  or is equal to 0
 */
void check_battery()
{
    if (battery > 0 && battery <= 25)
    {
        RTB = true;
    }

    else if (battery <= 0)
    {
        battery = 0;
        rdx = 0.0;
        rdy = 0.0;
        dead_battery();
    }
}

/**
 *  Tests to see if the payload weight is 45 grams or above
 */
void check_payload()
{
    if (grams >= 45)
    {
        RTB = true;
        ;
    }
}

/**
 *  A dialogue which allows the position and velocity of a designated robo to 
 *  be manually overridden.
 */
void do_robo_cheat()
{
    int x = get_int("x?");
    int y = get_int("y?");
    int angle = get_int("degrees?");
    heading = angle;

    rx = x;
    ry = y;
    rdx = -1 * ROBO_SPEED * cos(angle * M_PI / 180);
    rdy = ROBO_SPEED * sin(angle * M_PI / 180);
}

void do_base_cheat()
{
    RTB = true;
    return_to_base();
}

/**
 *  Allows user to change payload weight of robovac
 */
void do_weight_cheat()
{
    grams = get_int("enter payload weight in grams");
}

/**
 *  Allows user to change the remaining battery percentage
 */
void do_battery_cheat()
{
    battery = get_int("enter remaining battery in percentage");
}

void do_drop_dust()
{
    int x = get_int("x?");
    int y = get_int("y?");

    if (num_d == MAX_DUST)
    {
    }
    else if (num_d < MAX_DUST) {
        num_d++;
        dsx[num_d-1] = x;
        dsy[num_d-1] = y;
        count_d++;
        draw_all();
    }
        
}

    void do_drop_slime()
    {
        int x = get_int("x?");
    int y = get_int("y?");

    if (num_s == MAX_SLIME)
    {
    }
    else if (num_s < MAX_SLIME) {
        num_s++;
        sx[num_s-1] = x;
        sy[num_s-1] = y;
        count_s++;
        draw_all();
    }
    }

    void do_drop_litter()
    {
        int x = get_int("x?");
        int y = get_int("y?");

        if (num_l == MAX_LITTER)
        {
        }
        else if (num_l < MAX_LITTER)
        {

            num_l++;
            lx[num_l-1] = x;
            ly[num_l-1] = y;
            draw_all();
            count_l++;
        }
    }

    /**
 *  Toggles robo motion between paused and not-paused.
 */
    void do_pause()
    {
        paused = !paused;
    }

    /**
 *  Dialogue which allows the loop delay to be manually overridden.
 */
    void do_delay_cheat()
    {
        delay = get_int("New delay (milliseconds)?");

        if (delay < 0)
            delay = 10;
    }

    /**
 *  Dialog which displays a help screen.
 */
    void do_help()
    {
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
                             "o: time out for a user-specified number of milliseconds\n"
                             "v: Robovac movement cheat - override the position and heading of the Robovac\n"
                             "w: Robovac weight cheat - override the current payload weight of the Robovac\n"
                             "y: Robovac battery cheat - override the current remaining battery percentage of the Robovac\n"
                             "p: pause or resume Robovac movement\n"
                             "r: restart\n"
                             "q: quit\n"
                             "?: display this help screen\n\n\n"
                             "Press a key to return to the game.");
        show_screen();

        while (get_char() > 0)
        {
        }
        while (!timed_out() && wait_char() <= 0)
        {
        }
    }

    /**
 *  Jump table which chooses the action (if any) which corresponds to a 
 *  supplied command character.
 *
 *  Parameters:
 *      ch: a command character. Currently recognised comands are listed in 
 *          the do_help function.
 */
    void do_operation(int ch)
    {
        if (ch == 'r')
        {
            reset();
        }
        else if (is_robo_ctrl(ch))
        {
            move_robo(ch);
        }
        else if (ch == 'o')
        {
            do_timeout();
        }
        else if (ch == 'v')
        {
            do_robo_cheat();
        }
        else if (ch == 'b')
        {
            do_base_cheat();
        }
        else if (ch == 'w')
        {
            do_weight_cheat();
        }
        else if (ch == 'y')
        {
            do_battery_cheat();
        }
        else if (ch == 'd')
        {
            do_drop_dust();
        }
        else if (ch == 's')
        {
            do_drop_slime();
        }
        else if (ch == 't')
        {
            do_drop_litter();
        }
        else if (ch == 'p')
        {
            do_pause();
        }
        else if (ch == 'm')
        {
            do_delay_cheat();
        }
        else if (ch == '?')
        {
            do_help();
        }
    }

    /**
 *  The loop function: called once per time slice to run the simulation.
 */
    void loop()
    {
        int ch = get_char();

        if (ch == 'q')
        {
            do_game_over();
        }
        else if (ch == 'r')
        {
            reset();
        }
        else if (ch >= ' ')
        {
            do_operation(ch);
        }
        else if (!paused)
        {
            update_robo();
        }

        check_battery();
        charge_battery();
        check_payload();
        return_to_base();
        HUD_calcs();
        collect_trash();
        draw_all();
    }

    /**
 *  Program start and event loop.
 */
    int main(void)
    {

        srand(get_current_time());
        setup_screen();
        setup();

        while (!game_over && !timed_out())
        {
            loop();
            timer_pause(delay);
        }

        return 0;
    }