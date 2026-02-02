/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#ifndef RAYLIB_H
#include "raylib.h"
#endif // RAYLIB_H

#include "draw_character.h"
#include "resource_dir.h"	// utility header for SearchAndSetResourceDir
#include <stdlib.h>
#include <time.h>

//declare game state data
typedef enum {TITLE, BATTLE_START, BATTLE_HAPPENING, BATTLE_END} GameScreen;

const int screen_width = 1600;
const int screen_height = 1280;

//declare enemy_ship_data
int explosion_counter;
int explosion_radius;
int explosion_duration;
Texture enemy_texture;
typedef enum {NONE, HOSTILE, FRIENDLY, EXPLOSION_START, EXPLODING, EXPLOSION_FADING, DEAD} EnemyShipState;
EnemyShipState enemy_state;
int enemy_centre_x;
int enemy_centre_y;


//declare text box data
int textbox_upper_left_x;
int textbox_upper_left_y;
int textbox_width;
int textbox_height; 
int textbox_text_size;
int text_start_x;
int text_start_y;
int text_height;

// state machine controlling enemy state
void enemy_ship_SM(){

	switch(enemy_state){
		case(HOSTILE):
		{
			DrawTexture(enemy_texture, enemy_centre_x -200, enemy_centre_y - 200, WHITE);
		} break;
		case(FRIENDLY):
		{
			DrawTexture(enemy_texture, enemy_centre_x -200, enemy_centre_y - 200, WHITE);
		} break;
		case(EXPLOSION_START):
		{
			explosion_counter = 1;
			enemy_state = EXPLODING;
		}break;
		
		case(EXPLODING):
		{
			DrawTexture(enemy_texture, enemy_centre_x -200, enemy_centre_y - 200, WHITE);
			DrawCircle(enemy_centre_x, enemy_centre_y, explosion_counter, ORANGE);
			explosion_counter= explosion_counter + 10;
			if(explosion_counter > explosion_radius) enemy_state = EXPLOSION_FADING;

		}break;
		case(EXPLOSION_FADING):
		{
			if (explosion_counter % 2 == 0) DrawCircle(enemy_centre_x, enemy_centre_y, explosion_radius, RED);
			explosion_counter++;
			if (explosion_counter > explosion_duration + explosion_radius) enemy_state = DEAD;
		}
		default:break;
	}
}

void draw_cold_void_textbox(){
	DrawRectangle(textbox_upper_left_x, textbox_upper_left_y, textbox_width, textbox_height, BLUE);
	DrawRectangle(textbox_upper_left_x + 10, textbox_upper_left_y + 10, textbox_width - 20, textbox_height - 20, BLACK);
}

void draw_cursor(int position){
	switch(position){
		case 0:
		{
		struct Vector2 a = {text_start_x- 40, text_start_y};
		struct Vector2 b = {text_start_x - 40, text_start_y + text_height};
		struct Vector2 c = {text_start_x - 20, text_start_y + text_height/2};
		DrawTriangle(a,b,c,WHITE);
		} break;
		case 1:
		{
		struct Vector2 a = {text_start_x- 40, text_start_y + text_height + 20};
		struct Vector2 b = {text_start_x - 40, text_start_y + text_height + text_height + 20};
		struct Vector2 c = {text_start_x - 20, text_start_y + text_height + 20 + text_height/2};
		DrawTriangle(a,b,c,WHITE);
		} break;
		default: break;
	}
}

void fire_lazers(float target_x_position, float target_y_position){
	struct Vector2 left_lazer_end_point = {target_x_position -10, target_y_position};
	struct Vector2 left_lazer_start_point_left = {target_x_position - 200, 900};
	struct Vector2 left_lazer_start_point_right = {target_x_position - 100, 900};

	struct Vector2 right_lazer_end_point = {target_x_position +10, target_y_position};
	struct Vector2 right_lazer_start_point_left = {target_x_position + 100 , 900};
	struct Vector2 right_lazer_start_point_right = {target_x_position +200, 900}; 
	
	DrawTriangle(left_lazer_end_point, left_lazer_start_point_left, left_lazer_start_point_right, ORANGE);
	DrawTriangle(right_lazer_end_point, right_lazer_start_point_left, right_lazer_start_point_right, ORANGE);

}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(screen_width, screen_height, "Cold Void");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// Initialise Text box
	textbox_upper_left_x = 10;
	textbox_upper_left_y = 900;
	textbox_width = 1580;
	textbox_height = 370; 
	textbox_text_size = 50;
	text_start_x = textbox_upper_left_x + 100;
	text_start_y = textbox_upper_left_y + 100;
	text_height = 50;	
	// Initialise Face Variables
	int face_centre_x = screen_width*3/4;
	int face_centre_y = screen_height/3;
	Color face_skin_colour = BROWN;
	srand(time(NULL)); //seed random number generator with the time
	int eye_width = 40 + rand() % 120;
	int eye_height = 20 + rand() % 100;
	
	int iris_size_off = rand() % 10;
    int pupil_size_off = rand() % 10;
	int eeo = 20 + rand() % 120; //eye_eye_offset, sets the distance between eyes
	int eho = 40 + rand() % 40; //eye_head_offset, sets the distance between eyes and the side of the head
	int evo = -40 + rand() % 100; // eye vertical offset, sets where eye is vertically relative to centre of head

	//int centre_x_position = 800;  // used to reference centre of screen x position
	//int centre_y_position = 640;	// used to reference centre of screen y position
	
	//initialise title data
	int title_size = 600;
	int title_x_position = -400;
	int title_y_position = 400;
	
	char title_text[] = "Cold Void";
	GameScreen current_screen = TITLE;
	int frame_count = 0;
	int cursor_position = 0;
	

	
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		//game resets if the r button is pushed
		if(IsKeyPressed('R')) {
			title_size = 600;

			title_x_position = -400;
			title_y_position = 400;

			current_screen = TITLE;
			frame_count = 0;
			cursor_position = 0;
			eye_width = 40 + rand() % 120;
			eye_height = 20 + rand() % 100;
			iris_size_off = rand() % 10;
            pupil_size_off = rand() % 10;
			eeo = 20 + rand() % 120;
	        eho = 40 + rand() % 40;
			evo = -40 + rand() % 100;

			
		}

		switch (current_screen){
			case TITLE:
			{
				//DrawText("TITLE", 0, 0, 100, RED);
				//DrawText("+", centre_x_position, centre_y_position, 100, RED); //work out where the centre is
				//title colapse inward
				if(title_size > 10)	{
					
					
					// draw some text using the default font
					DrawText(title_text, title_x_position, title_y_position,title_size,WHITE);
					// progressively make title size smaller
					
					if(frame_count % 2 == 0) {
						//title_x_position = title_x_position +1;
						title_y_position = title_y_position +1;
					}
					title_x_position = title_x_position +2;
					title_size--;
				}
				
				if (IsKeyPressed(KEY_ENTER))
				{
					current_screen = BATTLE_START;
					frame_count = 0;
				}
				
			} break;
			case BATTLE_START:
			{
			//	DrawText("BATTLE_START", 0, 0, 100, RED);
				//initialise enemy data
				enemy_state = NONE;
				explosion_counter = 0;
				explosion_radius = 250;
				explosion_duration = 20;
				enemy_texture = LoadTexture("enemy_spaceship.png");
				enemy_centre_x = 600;
				enemy_centre_y = 600;

				draw_head(face_centre_x, face_centre_y, eeo, eho,evo, face_skin_colour, eye_height, eye_width, iris_size_off, pupil_size_off);
				draw_cold_void_textbox();
				DrawText("WHAT SHOULD WE DO CAPTAIN!", text_start_x, text_start_y, textbox_text_size, WHITE);
				DrawText("(PRESS SPACE TO CONTINUE)", text_start_x, text_start_y + text_height + 20, textbox_text_size, WHITE);
			
				if(IsKeyReleased(' ')){
					enemy_state = HOSTILE;
					current_screen = BATTLE_HAPPENING;
				}

			} break;
			case BATTLE_HAPPENING:
			{	
			//	DrawText("BATTLE_HAPPENING", 0, 0, 100, RED);
				draw_head(face_centre_x, face_centre_y, eeo, eho,evo, face_skin_colour, eye_height, eye_width, iris_size_off, pupil_size_off);
				//handle text box
				draw_cold_void_textbox();
				if(cursor_position == 0 && IsKeyReleased('S')){
					cursor_position = 1;
				}
				if(cursor_position == 1 && IsKeyReleased('W')){
					cursor_position = 0;
				}				

				draw_cursor(cursor_position);
				DrawText("FIRE", text_start_x, text_start_y, textbox_text_size, WHITE);
				DrawText("BE FRIENDS", text_start_x, text_start_y + text_height + 20, textbox_text_size, WHITE);

				//handle enemy state
				enemy_ship_SM();

				if(IsKeyDown(' ') && cursor_position == 0){
					fire_lazers(enemy_centre_x, enemy_centre_y);
					
				}
				if(IsKeyReleased(' ') && cursor_position == 0){
					enemy_state = EXPLOSION_START;
				}
				
				if(IsKeyReleased(' ') && cursor_position == 1){
					enemy_state = FRIENDLY;
				}
				if(enemy_state == FRIENDLY || enemy_state == DEAD){
					current_screen = BATTLE_END;
				}
				
			} break;
			case BATTLE_END:
			{
			//	DrawText("BATTLE_END", 0, 0, 100, RED);
				draw_head(face_centre_x, face_centre_y, eeo, eho,evo, face_skin_colour, eye_height, eye_width, iris_size_off, pupil_size_off);
				enemy_ship_SM();
				draw_cold_void_textbox();
				DrawText("Well done!", text_start_x, text_start_y, textbox_text_size, WHITE);

            
			} break;
			default: break;
		}
		
		
		
		// end the frame and get ready for the next one  (display frame, poll input, etc...)
		EndDrawing();
		frame_count++;
	}

	// unload enemy texture
	UnloadTexture(enemy_texture);

	// destroy the window and cleanup the OpenGL context
	CloseWindow();
	return 0;
}
