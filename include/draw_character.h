void draw_eye(int tw, int in_w, int eoh, int in_h, int start_x, int start_y);
void draw_diamond_mouth(int pos_x, int pos_y, int out_w, int in_w, int out_h, int in_h, Color teeth_colour,
                 Color skin_colour );
void draw_nose(float start_x, float start_y, float top_h, float bot_h, float width, Color skin_colour);
void draw_head(int face_centre_x, int face_centre_y, int eye_head_off, int eye_eye_off, int eye_vert_off,
     Color face_skin_colour, int eye_height, int eye_width, int iris_size_offset, int pupil_size_offset);
void draw_square_jaw(int face_centre_x, int face_centre_y, double face_radius, int chin_width, int chin_height, Color colour);
void draw_diamond_eye(int pos_x, int pos_y, int out_w, int in_w, int out_h, int in_h,
                int iris_radius, int pupil_radius, Color sclera_colour, Color iris_colour, Color pupil_colour, Color skin_colour );