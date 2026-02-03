

typedef struct {
    int centre_x;
    int centre_y; 
    int eye_head_off;
    int eye_eye_off; //eye_eye_offset, sets the distance between the inner edges of the eyes
    int eye_vert_off;
    int eye_nose_off;
    int nose_mouth_off;
    int mouth_chin_x_off;
    int mouth_chin_y_off;
    int nose_upper_height;
    int nose_lower_height;
    int nose_width;
    int mouth_height;
    int mouth_out_width;
    int mouth_in_width;
    int eye_out_width_off;
    int eye_out_height_off;
    int eye_in_height_off;
    int eye_in_width_off;
    int iris_size_off;
    int pupil_radius;
    Color skin_colour;
    Color sclera_colour;
    Color iris_colour;
    Color pupil_colour;
    Color teeth_colour;
} Face;

int eye_in_w(Face);
int eye_out_w(Face);
int eye_in_h(Face);
int eye_out_h(Face);
int face_radius(Face);
int face_height(Face);

void draw_diamond_mouth(Face);
void draw_nose(Face);
void draw_head(Face);
void draw_square_jaw(Face);
void draw_diamond_eye(int pos_x, int pos_y, Face );