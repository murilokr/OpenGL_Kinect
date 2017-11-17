const char* WINDOW_TITLE = "OpenGL 3D HolyEngine";

int WINDOW_WIDTH = 1024;
int WINDOW_HEIGHT = 768;

#define SafeDelete(obj) \
    if(obj != NULL) { \
        delete obj; \
        obj = NULL; \
    }
