#include "raylib.h"
#include "draw_character.h"
#include "math.h"


void draw_diamond_eye(int pos_x, int pos_y, Face f)
{
    
    //DrawRectangle(pos_x, pos_y, out_w, out_h, skin_colour);  //eyes background
    
    DrawRectangle(pos_x + f.eye_out_width_off, pos_y + (eye_out_h(f)-eye_in_h(f))/2, eye_in_w(f), eye_in_h(f), f.sclera_colour); //draws sclera rectangle
    struct Vector2 la = {pos_x , pos_y + eye_out_h(f)/2}; // left triangle point a
	struct Vector2 lb = {pos_x + f.eye_out_width_off, pos_y + (eye_out_h(f) + eye_in_h(f))/2};
	struct Vector2 lc = {pos_x + f.eye_out_width_off, pos_y + (eye_out_h(f)-eye_in_h(f))/2};
	DrawTriangle(la,lb,lc,f.sclera_colour);
    struct Vector2 ra = {pos_x + eye_in_w(f) + f.eye_out_width_off , pos_y + (eye_out_h(f)-eye_in_h(f))/2};
    struct Vector2 rb = {pos_x + f.eye_out_width_off + eye_in_w(f), pos_y + (eye_out_h(f) + eye_in_h(f))/2};
    struct Vector2 rc = {pos_x + eye_out_w(f), pos_y + eye_out_h(f)/2};
    DrawTriangle(ra, rb, rc, f.sclera_colour);
    DrawCircle(pos_x + eye_out_w(f)/2, pos_y + eye_out_h(f)/2, f.pupil_radius + f.iris_size_off, f.iris_colour);  // draw iris
    
    DrawCircle(pos_x + eye_out_w(f)/2, pos_y + eye_out_h(f)/2, f.pupil_radius, f.pupil_colour);   //draw pupil
    
    DrawRectangle(pos_x, pos_y, eye_out_w(f), (eye_out_h(f) -eye_in_h(f))/2, f.skin_colour); // top cover
    DrawRectangle(pos_x, pos_y+ eye_out_h(f)/2 + eye_in_h(f)/2, eye_out_w(f), (eye_out_h(f) -eye_in_h(f))/2 +4, f.skin_colour); // bottom cover
    //DrawText("x",rsp_x+out_w/2, rsp_y+out_h/2, 30, RED );

}

int eye_in_w(Face f){
    return f.eye_in_width_off + f.iris_size_off*2 + f.pupil_radius*2; // length of inner rectangle
}

int eye_out_w(Face f){
    return f.eye_out_width_off*2 + f.eye_in_width_off + f.iris_size_off*2 + f.pupil_radius*2;
}

int eye_in_h(Face f){
    return f.eye_in_height_off + f.pupil_radius*2; // ensure you can see the whole pupil
}

int eye_out_h(Face f){
    return f.iris_size_off*2 + f.pupil_radius*2;
}

int face_radius(Face f){
    return f.eye_eye_off + f.eye_head_off + eye_out_w(f)*2;
}

int face_height(Face f){
    return eye_out_h(f) + f.eye_vert_off + f.eye_nose_off + f.nose_upper_height
                         + f.nose_lower_height + f.nose_mouth_off + f.mouth_height + f.nose_mouth_off;
}


void draw_diamond_mouth(Face f)
{
    
    int pos_x = f.centre_x - f.mouth_out_width/2; // centre the mouth
    int pos_y = f.centre_y + f.eye_vert_off + eye_out_h(f) + f.eye_nose_off + f.nose_upper_height + f.nose_lower_height + f.nose_mouth_off;
    
    DrawRectangle(pos_x + (f.mouth_out_width - f.mouth_in_width)/2, pos_y, f.mouth_in_width, f.mouth_height, f.teeth_colour);
    struct Vector2 la = {pos_x , pos_y + f.mouth_height/2}; // left triangle point a
	struct Vector2 lb = {pos_x + (f.mouth_out_width - f.mouth_in_width)/2, pos_y + f.mouth_height};
	struct Vector2 lc = {pos_x + (f.mouth_out_width - f.mouth_in_width)/2, pos_y};
	DrawTriangle(la,lb,lc, f.teeth_colour);
    struct Vector2 ra = {pos_x + (f.mouth_out_width + f.mouth_in_width)/2 , pos_y};
    struct Vector2 rb = {pos_x + (f.mouth_out_width + f.mouth_in_width)/2, pos_y + f.mouth_height};
    struct Vector2 rc = {pos_x+ f.mouth_out_width, pos_y + f.mouth_height/2 };
    DrawTriangle(ra, rb, rc, f.teeth_colour);

}


void draw_nose(Face f)
{
    // calculate nose start position
    int start_x = f.centre_x;
    int start_y = f.centre_y + f.eye_vert_off + eye_out_h(f) + f.eye_nose_off;

    
    struct Vector2 ta = {start_x, start_y};
    struct Vector2 tb = {start_x + f.nose_width, start_y + f.nose_upper_height};
    DrawLineV(ta, tb, BLACK);
    struct Vector2 tx = {start_x, start_y + f.nose_upper_height};
    struct Vector2 bx = {start_x, start_y + f.nose_upper_height + f.nose_lower_height};
    DrawTriangle(tx, bx, tb, BLACK);

}

void draw_square_jaw(Face f)
{
    int chin_width = f.mouth_out_width + f.mouth_chin_x_off*2;
    // the head will be built out of 4 pieces, a circle and three trianges
    // the three triangles all start from the centre of the circle, point a
    struct Vector2 a = {f.centre_x, f.centre_y};
  //  DrawText("A", face_centre_x, face_centre_y, 30, RED);
    // then there are two points either side of the bottom of the jaw
    struct Vector2 b = {f.centre_x - chin_width/2, f.centre_y + face_height(f)};
   // DrawText("B", face_centre_x - chin_width/2, face_centre_y + chin_height, 30, RED);
    struct Vector2 c = {f.centre_x + chin_width/2, f.centre_y + face_height(f)};
  //  DrawText("C",face_centre_x + chin_width/2, face_centre_y + chin_height, 30, RED);
    DrawTriangle(a, b, c, f.skin_colour);
    // then there are two points, either side of the head where the jaw meets the head
    // to make sure they connect smoothly the triangle need to be tangent to the circle
    
    // first calculate the distance from the centre of the circle to the bottom corner of the jaw
    // in x direction
    double dist = sqrt((chin_width/2)*(chin_width/2) + face_height(f)*face_height(f)); 
    // two angles must be calculated, the angle between the line from the face centre to point c
    // and the centre line of the circle
    double theta = atan2(f.centre_y + face_height(f), f.centre_x + chin_width/2);
    //double theta = acos((face_centre_x + chin_width/2)/dist);
    // and the angle between the line to the tangent point and the line to point c
    double phi = acos(face_radius(f)/dist);
    struct Vector2 d = {f.centre_x + face_radius(f)*cos(theta - phi), f.centre_y - face_radius(f)*sin(theta - phi)};
  //  DrawText("D", face_radius*cos(theta - phi), face_radius*sin(theta - phi), 30, RED);
    
    DrawTriangle(a, c, d, f.skin_colour);  // right jaw triangle
    // the face is a mirror symmetric, as faces are
    struct Vector2 e = {f.centre_x-face_radius(f)*cos(theta - phi), f.centre_y - face_radius(f)*sin(theta - phi)};
    DrawTriangle(e, b, a, f.skin_colour); // left jaw triangle
    
}

void draw_head(Face f)
{
    
    DrawCircle(f.centre_x, f.centre_y, face_radius(f), f.skin_colour);

    draw_square_jaw(f);

    draw_diamond_eye(f.centre_x + f.eye_eye_off/2, f.centre_y + f.eye_head_off, f);

	draw_diamond_eye(f.centre_x - eye_out_w(f) - f.eye_eye_off, f.centre_y + f.eye_head_off, f);
				
	draw_nose(f);

	draw_diamond_mouth(f);
}

