#include <stdlib.h>
#include <math.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

#define DELAY 10 /* milliseconds */

bool game_over;
bool update_screen = true;

//  (a)
sprite_id station;
sprite_id star;

char * station_image = "9";
char * star_image = "*";

int x_star;
int y_star;

int x_station = 10;
int y_station = 6;

double velocity = 0.22;

void accelerate_station_towards_star(void);

void setup(void) {
    //  (b)
    x_star = screen_width() / 2;
    y_star = screen_height() / 2;

    //  (c)
    station = sprite_create(x_station, y_station, 1, 1, station_image);
    star = sprite_create(x_star, y_star, 1, 1, star_image);
    
    //  (d)
    double dx = x_star - x_station;
    

    //  (e)
    double dy = y_star - y_station;

    //  (f)
    double dist = sqrt((dx * dx) + (dy + dy));

    //  (g)
    dx = dx / dist;
    dy = dy / dist;

    //  (h)
    dx = dx * velocity;
    dy = dy * velocity;

    //  (i)
    sprite_turn_to(station, dx, dy);

    //  (j)
    sprite_turn(station, -90);

    //  (k)
    sprite_draw(station);

    //  (l)
    sprite_draw(star);
}

void process(void) {
    clear_screen();

    //  (m)
    accelerate_station_towards_star();

    //  (n)
    sprite_step(station);

    //  (o)
    sprite_draw(station);

    //  (p)
    sprite_draw(star);
}

void accelerate_station_towards_star(void) {
    //  (aa)
    double x_diff = sprite_x(star) - sprite_x(station);
    double y_diff = sprite_y(star) - sprite_y(station);

    //  (ab)
    double dist_squared = ((x_diff * x_diff) + (y_diff * y_diff));

    //  (ac)
    if (dist_squared < 1e-10) {
        dist_squared = 1e-10;
    }

    //  (ad)
    double dist = sqrt(dist_squared);

    //  (ae)
    double dx = sprite_dx(station);
    double dy = sprite_dy(station);

    //  (af)
    int GM = 1;
    double a = GM / dist_squared;

    //  (ag)
    dx = dx + a * x_diff / dist;
    dy = dy + a * y_diff / dist;

    //  (ah) 
    double V = sqrt((dx * dx) + (dy * dy));

    //  (ai)
    if (V > 1) {
        dx = dx / V;
        dy = dy / V;
    }

    //  (aj)
    sprite_turn_to(station, dx, dy);
}

// Clean up game
void cleanup(void) {
    // STATEMENTS
}

// Program entry point.
int main(void) {
    setup_screen();

#if defined(AUTO_SAVE_SCREEN)
    auto_save_screen(true);
#endif

    setup();
    show_screen();

    while ( !game_over ) {
        process();

        if ( update_screen ) {
            show_screen();
        }

        timer_pause(DELAY);
    }

    cleanup();

    return 0;
}
