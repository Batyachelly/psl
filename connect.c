#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <pthread.h>
#include <curl/curl.h>

#include "include/connect.h"

void connect_get(struct connect *);
void connect_free(struct connect *);
void connect_stop(struct connect *);

void *connect_thread(void *arg)
{
    struct connect *connect = arg;
    // CURL *curl;
    CURLcode res;

    curl_easy_setopt(connect->curl, CURLOPT_URL, connect->url);
    curl_easy_setopt(connect->curl, CURLOPT_FOLLOWLOCATION, 1L);
    FILE *f = fopen(connect->filename, "wb");
    curl_easy_setopt(connect->curl, CURLOPT_WRITEDATA, f);

    res = curl_easy_perform(connect->curl);
    log_c("Curl connection was closed", __func__, __LINE__, 0);
    if (res != CURLE_OK)
    {
        // log_c("Curl was cleaned", __func__, __LINE__, 1);
    }else {
        log_c("Curl connection error", __func__, __LINE__, 2);
    }

    /* always cleanup */
    curl_easy_cleanup(connect->curl);
}

struct connect *connect_init(char *filename, char *url)
{
    struct connect *connect = malloc(sizeof(struct connect));
    connect->get = connect_get;
    connect->free = connect_free;
    connect->stop = connect_stop;
    connect->curl = curl_easy_init();
    if (connect->curl)
    {
        log_c("Curl was inited", __func__, __LINE__, 1);
        strcpy(connect->filename, filename);
        strcpy(connect->url, url);
    }
    else
    {
        log_c("Curl was not inited", __func__, __LINE__, 2);
    }
    return connect;
}

void connect_free(struct connect *connect)
{
    pthread_cancel(connect->connect_id);
    curl_easy_cleanup(connect->curl);
    free(connect);
    log_c("Curl was cleaned", __func__, __LINE__, 1);
}

void connect_get(struct connect *connect)
{
    log_c("Curl start thread", __func__, __LINE__, 1);
    pthread_create(&connect->connect_id, NULL, connect_thread, connect);
}

void connect_stop(struct connect *connect)
{
    pthread_cancel(connect->connect_id);
}
