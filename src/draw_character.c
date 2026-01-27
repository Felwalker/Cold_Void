#include "raylib.h"
#include "draw_character.h"


void draw_eye(){
    int rsp_x = 500;  // Rectangle Start Position x
    int rsp_y = 200; // Rectangle Start Position y
    int rw = 200; // Rectangle Width
    int rh = 100; // Rectangle Height 
    int tw = 50; // triangle width
    int eoh = 20; // eye over hang, how much the centre of the eye overhangs the open eye

    DrawRectangle(rsp_x -tw, rsp_y - eoh, rw+tw*2, rh +eoh*2, BROWN);  //eyes background
    
    DrawRectangle(rsp_x, rsp_y, rw, rh, WHITE);
    struct Vector2 la = {rsp_x -tw, rsp_y +rh/2}; // left triangle point a
	struct Vector2 lb = {rsp_x, rsp_y};
	struct Vector2 lc = {rsp_x, rsp_y + rh};
	DrawTriangle(lb,la,lc,WHITE);
    struct Vector2 ra = {rsp_x + rw, rsp_y};
    struct Vector2 rb = {rsp_x + rw + tw, rsp_y + rh/2};
    struct Vector2 rc = {rsp_x + rw, rsp_y + rh};
    DrawTriangle(ra, rc, rb, WHITE);
    DrawCircle(rsp_x+rw/2, rsp_y+rh/2, rh/2 + eoh, BLACK);

    DrawRectangle(rsp_x, rsp_y -eoh, rw, eoh, BROWN); // top cover
    DrawRectangle(rsp_x, rsp_y + rh, rw, eoh, BROWN); // bottom cover


}