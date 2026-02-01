#include "raylib.h"
#include "draw_character.h"
#include "math.h"




void draw_diamond_eye(int pos_x, int pos_y, int out_w, int in_w, int out_h, int in_h,
                int iris_size_offset, int pupil_size_offset, Color sclera_colour, Color iris_colour, Color pupil_colour, Color skin_colour ){

    DrawRectangle(pos_x, pos_y, out_w, out_h, skin_colour);  //eyes background
    
    DrawRectangle(pos_x + (out_w - in_w)/2, pos_y, in_w, out_h, sclera_colour);
    struct Vector2 la = {pos_x , pos_y + out_h/2}; // left triangle point a
	struct Vector2 lb = {pos_x + (out_w - in_w)/2, pos_y + out_h};
	struct Vector2 lc = {pos_x + (out_w - in_w)/2, pos_y};
	DrawTriangle(la,lb,lc,sclera_colour);
    struct Vector2 ra = {pos_x + (out_w + in_w)/2 , pos_y};
    struct Vector2 rb = {pos_x + (out_w + in_w)/2, pos_y + out_h};
    struct Vector2 rc = {pos_x+ out_w,pos_y + out_h/2 };
    DrawTriangle(ra, rb, rc, sclera_colour);
    DrawCircle(pos_x + out_w/2, pos_y+out_h/2, out_h/2 - iris_size_offset, iris_colour);  // draw iris
    
    DrawCircle(pos_x + out_w/2, pos_y+out_h/2, out_h/2 - iris_size_offset - pupil_size_offset, pupil_colour);   //draw pupil
    
    DrawRectangle(pos_x, pos_y, out_w, in_h/2, skin_colour); // top cover
    DrawRectangle(pos_x, pos_y+ out_h -in_h/2, out_w, in_h/2, skin_colour); // bottom cover
    //DrawText("x",rsp_x+out_w/2, rsp_y+out_h/2, 30, RED );

}

void draw_diamond_mouth(int pos_x, int pos_y, int out_w, int in_w, int out_h, int in_h, Color teeth_colour,
                 Color skin_colour ){
    int tw = (out_w - in_w)/2; // tw stands for triangle width (at the eye's corner)
    int eoh = (out_h - in_h)/2; // eoh stands for eye over hang hie
    int rsp_x = pos_x + tw;  // Rectangle Start Position x
    int rsp_y = pos_y + eoh; // Rectangle Start Position y

    DrawRectangle(rsp_x -tw, rsp_y - eoh, in_w+tw*2, in_h +eoh*2, skin_colour);  //eyes background
    
    DrawRectangle(rsp_x, rsp_y, in_w, in_h, teeth_colour);
    struct Vector2 la = {rsp_x -tw, rsp_y +in_h/2}; // left triangle point a
	struct Vector2 lb = {rsp_x, rsp_y};
	struct Vector2 lc = {rsp_x, rsp_y + in_h};
	DrawTriangle(lb,la,lc,teeth_colour);
    struct Vector2 ra = {rsp_x + in_w, rsp_y};
    struct Vector2 rb = {rsp_x + in_w + tw, rsp_y + in_h/2};
    struct Vector2 rc = {rsp_x + in_w, rsp_y + in_h};
    DrawTriangle(ra, rc, rb, teeth_colour);

    DrawRectangle(rsp_x, rsp_y -eoh, in_w, eoh, skin_colour); // top cover
    DrawRectangle(rsp_x, rsp_y + in_h, in_w, eoh, skin_colour); // bottom cover

}


void draw_nose(float start_x, float start_y, float top_h, float bot_h, float width, Color skin_colour){
    DrawRectangle(start_x, start_y, width, top_h + bot_h, skin_colour);
    struct Vector2 ta = {start_x, start_y};
    struct Vector2 tb = {start_x + width, start_y + top_h};
    DrawLineV(ta, tb, BLACK);
    struct Vector2 tx = {start_x, start_y + top_h};
    struct Vector2 bx = {start_x, start_y + top_h + bot_h};
    DrawTriangle(tx, bx, tb, BLACK);

}

void draw_square_jaw(int face_centre_x, int face_centre_y, double face_radius, int chin_width, int chin_height, Color colour){
    // the head will be built out of 4 pieces, a circle and three trianges
    // the three triangles all start from the centre of the circle, point a
    struct Vector2 a = {face_centre_x, face_centre_y};
  //  DrawText("A", face_centre_x, face_centre_y, 30, RED);
    // then there are two points either side of the bottom of the jaw
    struct Vector2 b = {face_centre_x - chin_width/2, face_centre_y + chin_height};
   // DrawText("B", face_centre_x - chin_width/2, face_centre_y + chin_height, 30, RED);
    struct Vector2 c = {face_centre_x + chin_width/2, face_centre_y + chin_height};
  //  DrawText("C",face_centre_x + chin_width/2, face_centre_y + chin_height, 30, RED);
    DrawTriangle(a, b, c, colour);
    // then there are two points, either side of the head where the jaw meets the head
    // to make sure they connect smoothly the triangle need to be tangent to the circle
    
    // first calculate the distance from the centre of the circle to the bottom corner of the jaw
    // in x direction
    double dist = sqrt((chin_width/2)*(chin_width/2) + chin_height*chin_height); 
    // two angles must be calculated, the angle between the line from the face centre to point c
    // and the centre line of the circle
    double theta = atan2(face_centre_y + chin_height, face_centre_x + chin_width/2);
    //double theta = acos((face_centre_x + chin_width/2)/dist);
    // and the angle between the line to the tangent point and the line to point c
    double phi = acos(face_radius/dist);
    struct Vector2 d = {face_centre_x+face_radius*cos(theta - phi), face_centre_y + face_radius*sin(theta - phi)};
  //  DrawText("D", face_radius*cos(theta - phi), face_radius*sin(theta - phi), 30, RED);
    
    DrawTriangle(a, c, d, colour);
    // the face is a mirror symmetric, as faces are
    struct Vector2 e = {face_centre_x-face_radius*cos(theta - phi), face_centre_y + face_radius*sin(theta - phi)};
    DrawTriangle(e, b, a, colour);
    
}

void draw_head(int face_centre_x, int face_centre_y, int face_radius, Color face_skin_colour, int eye_height, int eye_width, int iris_size_offset, int pupil_size_offset){
  
    
    //int iris_size_off = 5;
    //int pupil_size_off = 5;
    DrawCircle(face_centre_x, face_centre_y, face_radius, face_skin_colour);
            	draw_square_jaw(face_centre_x, face_centre_y, face_radius, face_radius, eye_height + eye_width + eye_height + eye_height*2, face_skin_colour);
            	draw_diamond_eye(face_centre_x - eye_width - 40, face_centre_y, eye_width, eye_width*3/4, eye_height, 
					eye_height/2, iris_size_offset, pupil_size_offset, WHITE, BLUE, BLACK, face_skin_colour);
				draw_diamond_eye(face_centre_x + 40, face_centre_y, eye_width, eye_width*3/4, eye_height, 
					eye_height/2, iris_size_offset, pupil_size_offset, WHITE, BLUE, BLACK, face_skin_colour);
				
				draw_nose(face_centre_x, face_centre_y + eye_height, eye_width, eye_height, eye_height, face_skin_colour);
				draw_diamond_mouth(face_centre_x - eye_width/2, face_centre_y + eye_height + eye_width + eye_height + eye_height, eye_width, eye_width*3/4, eye_height, eye_height/2, WHITE, face_skin_colour);
}

