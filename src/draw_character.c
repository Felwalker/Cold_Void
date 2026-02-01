#include "raylib.h"
#include "draw_character.h"
#include "math.h"


void draw_diamond_eye(int pos_x, int pos_y, int out_w, int in_w, int out_h, int in_h,
                int iris_radius, int pupil_radius, Color sclera_colour, Color iris_colour, Color pupil_colour, Color skin_colour ){
    int tw = (out_w - in_w)/2; // tw stands for triangle width (at the eye's corner)
    int eoh = (out_h - in_h)/2; // eoh stands for eye over hang hie
    int rsp_x = pos_x + tw;  // Rectangle Start Position x
    int rsp_y = pos_y + eoh; // Rectangle Start Position y

    DrawRectangle(rsp_x -tw, rsp_y - eoh, in_w+tw*2, in_h +eoh*2, skin_colour);  //eyes background
    
    DrawRectangle(rsp_x, rsp_y, in_w, in_h, sclera_colour);
    struct Vector2 la = {rsp_x -tw, rsp_y +in_h/2}; // left triangle point a
	struct Vector2 lb = {rsp_x, rsp_y};
	struct Vector2 lc = {rsp_x, rsp_y + in_h};
	DrawTriangle(lb,la,lc,sclera_colour);
    struct Vector2 ra = {rsp_x + in_w, rsp_y};
    struct Vector2 rb = {rsp_x + in_w + tw, rsp_y + in_h/2};
    struct Vector2 rc = {rsp_x + in_w, rsp_y + in_h};
    DrawTriangle(ra, rc, rb, sclera_colour);
    DrawCircle(rsp_x+in_w/2, rsp_y+in_h/2, in_h/2 + eoh, iris_colour);
    DrawCircle(rsp_x+in_w/2, rsp_y+in_h/2, in_h/2 + 3, pupil_colour);

    DrawRectangle(rsp_x, rsp_y -eoh, in_w, eoh, skin_colour); // top cover
    DrawRectangle(rsp_x, rsp_y + in_h, in_w, eoh, skin_colour); // bottom cover

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

/** head drawing function, still kinda broken which is why it is commented out
void draw_head(int start_x, int start_y, int radius, int height, int type, 
    int eye_width, int eye_height, int iris_size, int pupil_size, int eye_ratio,
    int eye_eye_sep, int eye_nose_sep, int nose_mouth_sep, int mouth_chin_sep,  int chin_width,
     Color sclera_colour, Color iris_colour, Color pupil_colour, Color skin_colour, Color teeth_colour ){
    switch(type){
        case (0):{
            struct Rectangle r = {start_x, start_y, radius*2, height};
            DrawRectangleRounded(r, 0.8, 10, skin_colour); 
        }break;
        case(1):
        {
            
            DrawCircle(start_x + radius, start_y + radius, radius, skin_colour);
            draw_square_jaw(start_x + radius, start_y + radius, radius, chin_width, height/2, skin_colour);
            draw_diamond_eye(start_x + radius - eye_eye_sep/2, start_y + radius, eye_width/eye_ratio, eye_width*((eye_ratio-2)/eye_ratio),
               
                   sclera_colour, iris_colour, pupil_colour, skin_colour);
            draw_diamond_eye(start_x+width/5+width/5+width/5, start_y + height/2,
                width/5, width/8,
                 width/10, width/20,
                  width/10, width/20,
                   sclera_colour, iris_colour, pupil_colour, skin_colour);
            
          
            struct Vector2 a = {start_x, start_y + height/2}; // left triangle point a
            
	        struct Vector2 b = {start_x + width/2, start_y + height};
           
	        struct Vector2 c = {start_x + width, start_y + height/2};
            //DrawTriangle(a, b, c, YELLOW);
            //DrawText("X", start_x, start_y, 30, RED);
            //DrawText("X", start_x + width/2, start_y + height/2, 30, RED);
            //DrawText("A",start_x,  start_y + height/2, 30, RED);
            //DrawText("B", start_x + width/2, start_y + height, 30, RED);
            //DrawText("C", start_x + width, start_y + height/2, 30, RED);

            draw_diamond_mouth(start_x+width/2-width/5+width/10, start_y + width,
                width/5, width/8,
                 width/10, width/20,
                   teeth_colour, skin_colour);

            draw_nose(start_x + width/2, start_y + width/2, width/5, width/10, width/10, skin_colour);
            
        }break;
        default:break;
    }


           
}
    */