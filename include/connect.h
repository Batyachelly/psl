#include <curl/curl.h>

struct connect
{
    void (*get)(struct connect*);
    void (*free)(struct connect*);
    void (*stop)(struct connect*);
    CURL *curl;
    pthread_t connect_id;
    char filename[30];
    char url[100];
};

struct connect* connect_init(char* filename, char* url);

void (*log_c)(char *, const char *, int, int);