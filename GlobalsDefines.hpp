const char* WINDOW_TITLE    = "OpenGL 3D HolyEngine";

int WINDOW_WIDTH    = 1024;
int WINDOW_HEIGHT   = 768;

const double PI     = 3.14159265359;
const double TWO_PI = 6.28318530718;

#define SafeDelete(obj) \
    if(obj != NULL) { \
        delete obj; \
        obj = NULL; \
    }
