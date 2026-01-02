/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

typedef enum {TITLE, GAMEPLAY} GameScreen;

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(1600, 1280, "Cold Void");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	int title_x_position = -400;
	int title_y_position = 320;
	//Size of the Title
	int title_size = 600;
	char title_text[] = "Cold Void";
	GameScreen current_screen = TITLE;
	int frame_count = 0;
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		switch (current_screen){
			case TITLE:
			{
				//title colapse inward
				if(title_size > 10)	{
					// draw some text using the default font
					DrawText(title_text, title_x_position, title_y_position,title_size,WHITE);
					// progressively make title size smaller
					title_x_position++;
					title_y_position++;
					title_size--;
				}
				if (IsKeyPressed(KEY_ENTER))
				{
					current_screen = GAMEPLAY;
					frame_count = 0;
				}
				
			} break;
			case GAMEPLAY:
			{
				DrawText("Thought there would be more to it", 200, 200 , 50, WHITE);
				if(frame_count > 200) DrawText("Might as well press esc", 200, 260, 50, WHITE);
			} break;
			default: break;
		}
		
		
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
		frame_count++;
	}

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
