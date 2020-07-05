struct audioplay
{
    void (*play)(struct audioplay*);
    void (*free)(struct audioplay*);
    pthread_t audioplay_id;
    char filename[30];
};

struct audioplay* audioplay_init(char* filename);

void (*log_c)(char *, const char *, int, int);