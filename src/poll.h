#pragma once
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>

typedef struct {
    short int hour;
    short int minute;
    short int second;
} dt_status_t;

typedef struct {
    dt_status_t *status;
    int period_sec;
} passive_poll_args;

void *periodic_poll(void *args);
