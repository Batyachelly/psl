#include <stdlib.h>
#include <unistd.h>
#include <pthread.h>

#include "include/view.h"
#include "include/connect.h"
#include "include/audioplay.h"

struct view *view;

void log_handler(char text[], const char fun[], int line, int result){
    view->log_event(view, text, fun, line, result);
}

int main(int argc, char const *argv[])
{
    view = (struct view *)malloc(sizeof(struct view));

    pthread_t view_thread_id;
    pthread_create(&view_thread_id, NULL, view_thread, view);

    log_c = log_handler;

    usleep(900); //? Время на инициализацию курсов

    log_c("View start thread", __func__, __LINE__, 1);

    struct connect* connect = connect_init("t.mp3", "http://air.radiorecord.ru:805/ps_320");
    connect->get(connect);
    struct audioplay* audioplay = audioplay_init("t.mp3");
    audioplay->play(audioplay);

    pthread_join(view_thread_id, NULL);
    view->close(view);
    return 0;
}
