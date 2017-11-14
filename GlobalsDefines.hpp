#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 600


#define SafeDelete(obj) \
    if(obj != NULL) { \
        delete obj; \
        obj = NULL; \
    }
