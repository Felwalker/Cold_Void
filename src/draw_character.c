#include "raylib.h"
#include "draw_character.h"


void draw_eye(int tw, int in_w, int eoh, int in_h, int start_x, int start_y){
    int rsp_x = start_x + tw;  // Rectangle Start Position x
    int rsp_y = start_y + eoh; // Rectangle Start Position y

    DrawRectangle(rsp_x -tw, rsp_y - eoh, in_w+tw*2, in_h +eoh*2, BROWN);  //eyes background
    
    DrawRectangle(rsp_x, rsp_y, in_w, in_h, WHITE);
    struct Vector2 la = {rsp_x -tw, rsp_y +in_h/2}; // left triangle point a
	struct Vector2 lb = {rsp_x, rsp_y};
	struct Vector2 lc = {rsp_x, rsp_y + in_h};
	DrawTriangle(lb,la,lc,WHITE);
    struct Vector2 ra = {rsp_x + in_w, rsp_y};
    struct Vector2 rb = {rsp_x + in_w + tw, rsp_y + in_h/2};
    struct Vector2 rc = {rsp_x + in_w, rsp_y + in_h};
    DrawTriangle(ra, rc, rb, WHITE);
    DrawCircle(rsp_x+in_w/2, rsp_y+in_h/2, in_h/2 + eoh, BLACK);

    DrawRectangle(rsp_x, rsp_y -eoh, in_w, eoh, BROWN); // top cover
    DrawRectangle(rsp_x, rsp_y + in_h, in_w, eoh, BROWN); // bottom cover

}

void draw_mouth(int tw, int in_w, int eoh, int in_h, int start_x, int start_y){
    int rsp_x = start_x + tw;  // Rectangle Start Position x
    int rsp_y = start_y + eoh; // Rectangle Start Position y

    DrawRectangle(rsp_x -tw, rsp_y - eoh, in_w+tw*2, in_h +eoh*2, BROWN);  //eyes background
    
    DrawRectangle(rsp_x, rsp_y, in_w, in_h, WHITE);
    struct Vector2 la = {rsp_x -tw, rsp_y +in_h/2}; // left triangle point a
	struct Vector2 lb = {rsp_x, rsp_y};
	struct Vector2 lc = {rsp_x, rsp_y + in_h};
	DrawTriangle(lb,la,lc,WHITE);
    struct Vector2 ra = {rsp_x + in_w, rsp_y};
    struct Vector2 rb = {rsp_x + in_w + tw, rsp_y + in_h/2};
    struct Vector2 rc = {rsp_x + in_w, rsp_y + in_h};
    DrawTriangle(ra, rc, rb, WHITE);
    //DrawCircle(rsp_x+in_w/2, rsp_y+in_h/2, in_h/2 + eoh, BLACK);

    DrawRectangle(rsp_x, rsp_y -eoh, in_w, eoh, BROWN); // top cover
    DrawRectangle(rsp_x, rsp_y + in_h, in_w, eoh, BROWN); // bottom cover

}

void draw_nose(float start_x, float start_y, float top_h, float bot_h, float width){
    DrawRectangle(start_x, start_y, width, top_h + bot_h, BROWN);
    struct Vector2 ta = {start_x, start_y};
    struct Vector2 tb = {start_x + width, start_y + top_h};
    DrawLineV(ta, tb, BLACK);
    struct Vector2 tx = {start_x, start_y + top_h};
    struct Vector2 bx = {start_x, start_y + top_h + bot_h};
    DrawTriangle(tx, bx, tb, BLACK);

}

void draw_head(int start_x, int start_y, int width, int height){
    struct Rectangle r = {start_x, start_y, width, height};
    DrawRectangleRounded(r, 0.8, 10, BROWN);        
}