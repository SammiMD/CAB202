#include <stdlib.h>
#include <cab202_graphics.h>
#include <cab202_sprites.h>
#include <cab202_timers.h>

// Include additional headers here

// Constants used to configure game
#define DELAY (500) /* Millisecond delay between game updates */

// Add additional constants here

// Variables used to store the game’s dynamic state.
bool game_over = false; /* Set this to true when game is over */

// Define additional global variables here.

// Setup game.
void setup( void ) {
	// Insert statements here to set up game.
}


void process( void ) {
	int key_pressed = wait_char();

	clear_screen();
	draw_char( screen_width() / 2, screen_height() / 2, key_pressed );
}


// Clean up at end of game.
void cleanup( void ) {
	// Insert statements to release resources such as files, memory.
}

// Program entry point.
int main( void ) {
	setup_screen();  // Call ZDK function to prepare screen.
	setup();         // Call our setup function to initialise game state.
	show_screen();   // Display the screen at the start of play.

	// Following block will repeat until game_over becomes true.
	while ( ! game_over ) {
		process();            // Execute one time step of game.
		show_screen();        // Show user the results.
		timer_pause( DELAY ); // Let CPU rest a short time.
	}

	// Game over is now true, so clean up and exit.
	cleanup();

	return 0;
}