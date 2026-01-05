/*
Raylib example file.
This is an example main file for a simple raylib project.
Use this as a starting point or replace it with your code.

by Jeffery Myers is marked with CC0 1.0. To view a copy of this license, visit https://creativecommons.org/publicdomain/zero/1.0/

*/

#include "raylib.h"

#include "resource_dir.h"	// utility header for SearchAndSetResourceDir

typedef enum {TITLE, BATTLE_START, BATTLE_HAPPENING, BATTLE_END} GameScreen;

//bool debug_is_on = true;

//global text box position
int textbox_upper_left_x = 10;
int textbox_upper_left_y = 900;
int textbox_width = 1580;
int textbox_height = 370; 
int textbox_text_size = 50;
int text_start_x = textbox_upper_left_x + 100;
int text_start_y = textbox_upper_left_y + 100;
int text_height = 50;

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
	InitWindow(1600, 1280, "Cold Void");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	Texture enemy_texture = LoadTexture("enemy_spaceship.png");

	//int centre_x_position = 800;  // used to reference centre of screen x position
	//int centre_y_position = 640;	// used to reference centre of screen y position
	int title_size = 600;

	int title_x_position = -400;
	int title_y_position = 400;
	
	char title_text[] = "Cold Void";
	GameScreen current_screen = TITLE;
	int frame_count = 0;

	bool enemy_alive = true;
	bool now_friends = false;
	bool firing_lazers = false;
	bool explosion = false;
	int explosion_counter = 0;
	int enemy_x_pos = 600;
	int enemy_y_pos = 600;
	int cursor_position = 0;
	

	
	
	// game loop
	while (!WindowShouldClose())		// run the loop untill the user presses ESCAPE or presses the Close button on the window
	{
		// drawing
		BeginDrawing();

		// Setup the back buffer for drawing (clear color and depth buffers)
		ClearBackground(BLACK);

		if(IsKeyPressed('R')) {
			title_size = 600;

			title_x_position = -400;
			title_y_position = 400;

			current_screen = TITLE;
			frame_count = 0;
			enemy_alive = true;
			now_friends = false;
			cursor_position = 0;
			
		}

		switch (current_screen){
			case TITLE:
			{
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
					current_screen = BATTLE_HAPPENING;
					frame_count = 0;
				}
				
			} break;
			case BATTLE_HAPPENING:
			{	
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
				
				if(enemy_alive){
					DrawTexture(enemy_texture, enemy_x_pos -200, enemy_y_pos - 200, WHITE);
				} 
				if(IsKeyDown(' ') && cursor_position == 0){
					fire_lazers(enemy_x_pos, enemy_y_pos);
					
				}
				if(IsKeyReleased(' ') && cursor_position == 0){
					explosion = true;
				}
				if(IsKeyReleased(' ') && cursor_position == 1){
					now_friends = true;
				}
				if(explosion){
					explosion_counter++;
				
					if(explosion_counter > 10 && explosion_counter < 50){
						DrawCircle(enemy_x_pos, enemy_y_pos, 100, ORANGE);
					}
					else if(explosion_counter > 50 && explosion_counter < 100){
						DrawCircle(enemy_x_pos, enemy_y_pos, 200, ORANGE);
					}
					else if(explosion_counter > 100 && explosion_counter < 150){
						DrawCircle(enemy_x_pos, enemy_y_pos, 300, ORANGE);
						enemy_alive = false;
					}
					else if(explosion_counter > 150 && explosion_counter < 200 && explosion_counter % 2 == 0){
						DrawCircle(enemy_x_pos, enemy_y_pos, 300, RED);
					}
					else if (explosion_counter > 250){
						explosion = false;
						explosion_counter = 0;
					}
				}
				if(!enemy_alive && !explosion || enemy_alive && now_friends){
					current_screen = BATTLE_END;
				}
				
			} break;
			case BATTLE_END:
			{
				if(enemy_alive){
					DrawTexture(enemy_texture, enemy_x_pos -200, enemy_y_pos - 200, WHITE);
				}
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
