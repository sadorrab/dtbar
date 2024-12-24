#pragma once
#include <time.h>
#include <unistd.h>
#include <signal.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <pango/pangocairo.h>
#include "wl.h"


typedef struct {
    short int hour;
    short int minute;
    short int second;
    short int battery;
} dt_status_t;

typedef struct {
    dt_status_t *status;
    int period_sec;
} passive_poll_args;

void *periodic_poll(void *args);
