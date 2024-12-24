#include "poll.h"

void *periodic_poll(void *args) {
    passive_poll_args *ar = (passive_poll_args*) args;
    dt_status_t *status = ar->status;
    int sleeptime = ar->period_sec;

    time_t rawtime;
    struct tm *timeinfo;

    while (1) {
        time(&rawtime);
        timeinfo = localtime(&rawtime);
        status->hour = timeinfo->tm_hour;
        status->minute = timeinfo->tm_min;
        status->second = timeinfo->tm_sec;
        sleep(sleeptime);
    }
}

