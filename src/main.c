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
#include <stdio.h>
#include <curl/curl.h>
#include <string.h>
#include <cJSON.h>

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

void draw_text_wrapped(const char *text){
	// first we copy the first n characters to a substring
	int n = 0;
	int subtext_length = 55; // length of substring
	char subtext[subtext_length +1];
	for(int j = 0; j < 6; j++){
		for(int i = 0; i < subtext_length; i++){
			subtext[i] = text[i + n];
		}
		subtext[subtext_length] = '\0';
		DrawText(subtext, text_start_x, text_start_y + text_height*j , textbox_text_size, WHITE);
		n = n + subtext_length;
	}
		
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

struct Memory {
    char *response;
    size_t size;
};

// Callback function to handle the data received from the API
static size_t ResponseCallback(void *contents, size_t size, size_t nmemb, void *userp) {
    size_t totalSize = size * nmemb;
    struct Memory *mem = (struct Memory *)userp;

    printf(". %zu %zu\n", size, nmemb);
    char *ptr = realloc(mem->response, mem->size + totalSize + 1);
    if (ptr == NULL) {
        printf("Not enough memory to allocate buffer.\n");
        return 0;
    }

    mem->response = ptr;
    memcpy(&(mem->response[mem->size]), contents, totalSize);
    mem->size += totalSize;
    mem->response[mem->size] = '\0';

    return totalSize;
}


char *ollama_request(const char* req) {
    CURL *curl;
    CURLcode res;
    struct Memory chunk;

    chunk.response = malloc(1);  // Initialise memory
    chunk.size = 0;

    // Initialise libcurl
    curl = curl_easy_init();
    if(curl) {
        // Set the target URL
        curl_easy_setopt(curl, CURLOPT_URL, "http://localhost:11434/api/generate");

        // Specify the JSON data to send
        // Using "stream": false to get a single complete JSON response
        const char *json_data = req;
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, json_data);

        // Optional: Set Content-Type header to application/json
        struct curl_slist *headers = NULL;
        headers = curl_slist_append(headers, "Content-Type: application/json");
        curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

        /* 
           By default, libcurl writes the response to stdout.
           If you don't set CURLOPT_WRITEFUNCTION, it uses a default 
           callback that calls fwrite() on the 'stdout' stream.
        */

        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, ResponseCallback);

        curl_easy_setopt(curl, CURLOPT_WRITEDATA, (void *)&chunk);

        // Perform the request
        res = curl_easy_perform(curl);

        // Check for errors
        if(res != CURLE_OK) {
            fprintf(stderr, "curl_easy_perform() failed: %s\n", curl_easy_strerror(res));
        }
        else{
            printf("Response data: \n%s\n", chunk.response);
        }

        // Cleanup
        curl_slist_free_all(headers);
        curl_easy_cleanup(curl);
    }

    return chunk.response;
}


int get_msg_from_response(const char * const response, char* msg, size_t size)
{
   
    const cJSON *msg_json = NULL;
    int status = 0;
    cJSON *response_json = cJSON_Parse(response);
    if (response_json == NULL)
    {
        const char *error_ptr = cJSON_GetErrorPtr();
        if (error_ptr != NULL)
        {
            fprintf(stderr, "Error before: %s\n", error_ptr);
        }
        status = 0;
        goto end;
    }

    msg_json = cJSON_GetObjectItemCaseSensitive(response_json, "response");
    if (cJSON_IsString(msg_json) && (msg_json->valuestring != NULL))
    {
        strncpy(msg, msg_json->valuestring, size -1);
		msg[size -1] ; '\0';
		status =1;
    }
	else{
		status = 0;
		fprintf(stderr, "No response");
	}


end:
    cJSON_Delete(response_json);
    return status;
}

int main ()
{
	// Tell the window to use vsync and work on high DPI displays
	SetConfigFlags(FLAG_VSYNC_HINT | FLAG_WINDOW_HIGHDPI);

	// Create the window and OpenGL context
	InitWindow(screen_width, screen_height, "Cold Void");

	// Utility function from resource_dir.h to find the resources folder and set it as the current working directory so we can load from it
	SearchAndSetResourceDir("resources");

	// load_texture
	enemy_texture = LoadTexture("enemy_spaceship.png");

	// Initialise Text box
	textbox_upper_left_x = 10;
	textbox_upper_left_y = 900;
	textbox_width = 1580;
	textbox_height = 370; 
	textbox_text_size = 50;
	text_start_x = textbox_upper_left_x + 50;
	text_start_y = textbox_upper_left_y + 50;
	text_height = 50;	
	// Initialise Face Variables
	int face_centre_x = screen_width*3/4;
	int face_centre_y = screen_height/3;
	Color face_skin_colour = BROWN;
	srand(time(NULL)); //seed random number generator with the time
	
	Face guy = {
		.centre_x = screen_width*3/4,
    	.centre_y = screen_height/3,
    	.eye_head_off = 10 + rand()%40,
    	.eye_eye_off = 20 + rand()%40,
    	.eye_vert_off = 20 + rand()%40,
   		.eye_nose_off = 20 + rand()%40,
    	.nose_mouth_off = 20 + rand()%40,
    	.mouth_chin_x_off = 20 + rand()%40,
    	.mouth_chin_y_off = 20 + rand()%40,
    	.nose_upper_height = 20 + rand()%40,
    	.nose_lower_height = 20 + rand()%40,
   		.nose_width = 20 + rand()%40,
   		.mouth_height = 20 + rand()%40,
    	.mouth_out_width = 20 + rand()%40,
   		.mouth_in_width = 20 + rand()%40,
   		.eye_out_width_off = 10 + rand()%20,
    	.eye_in_height_off = 5 + rand()%10,
    	.eye_in_width_off = 5 + rand()%10,
    	.iris_size_off = 5 + rand()%10,
   		.pupil_radius = 5 + rand()%10,
    	.skin_colour = BROWN,
    	.sclera_colour = WHITE,
    	.iris_colour = BLUE,
    	.pupil_colour = BLACK,
		.teeth_colour = WHITE
	};
	
	//initialise title data
	int title_size = 600;
	int title_x_position = -400;
	int title_y_position = 400;
	
	char title_text[] = "Cold Void";
	GameScreen current_screen = TITLE;
	int frame_count = 0;
	int cursor_position = 0;

	//make first ollama_request for text and store it
	char request[] = "{\"model\": \"llama3\", \"prompt\": \"You are a character on a spaceship."
							 "You are informing the captain that we have encountered a spaceship.  Ask the captain"
							  "if they wish to prepare for battle or attempt to communicate.  Keep your response to"
							   "a single paragraph of less then 200 characters.\", \"stream\": false}";
	char *message_ptr = ollama_request(request);
	char first_words[1000];

	if( message_ptr != NULL) {
		get_msg_from_response(message_ptr, first_words, 1000);
		free(message_ptr);
	}

	


	

	
	
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
			// generate new face
    	guy.eye_head_off = 5 + rand()%10;
    	guy.eye_eye_off = 10 + rand()%20;
    	guy.eye_vert_off = 20 + rand()%40;
   		guy.eye_nose_off = 20 + rand()%40;
    	guy.nose_mouth_off = 20 + rand()%40;
    	guy.mouth_chin_x_off = 20 + rand()%40;
    	guy.mouth_chin_y_off = 20 + rand()%40;
    	guy.nose_upper_height = 20 + rand()%40;
    	guy.nose_lower_height = 20 + rand()%40;
   		guy.nose_width = 20 + rand()%40;
   		guy.mouth_height = 5 + rand()%25;
    	guy.mouth_out_width = 30 + rand()%60;
   		guy.mouth_in_width = 20 + rand()%40;
   		guy.eye_out_width_off = 10 + rand()%20;
    	guy.eye_in_height_off = 10 + rand()%20;
    	guy.eye_in_width_off = 10 + rand()%20;
    	guy.iris_size_off = 5 + rand()%10;
   		guy.pupil_radius = 5 + rand()%10;


			
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
				enemy_centre_x = 600;
				enemy_centre_y = 600;

				draw_head(guy);
				draw_cold_void_textbox();
				draw_text_wrapped(first_words);
				//DrawText(first_words, text_start_x, text_start_y, textbox_text_size, WHITE);
				//DrawText("WHAT SHOULD WE DO CAPTAIN!", text_start_x, text_start_y, textbox_text_size, WHITE);
				//DrawText("(PRESS SPACE TO CONTINUE)", text_start_x, text_start_y + text_height + 20, textbox_text_size, WHITE);
			
				if(IsKeyReleased(' ')){
					enemy_state = HOSTILE;
					current_screen = BATTLE_HAPPENING;
				}

			} break;
			case BATTLE_HAPPENING:
			{	
			//	DrawText("BATTLE_HAPPENING", 0, 0, 100, RED);
				draw_head(guy);
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
				draw_head(guy);				
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
