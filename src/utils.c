#include "utils.h"

dt_status_t* create_status() {
    dt_status_t *status = malloc(sizeof(dt_status_t));
    char icon_default[] = "0000";
    status->battery_icon = malloc(sizeof(char) * 5);
    strncpy(status->battery_icon, icon_default, 4);
    return status;
}

void destroy_status(dt_status_t *status) {
    free(status);
}

void status_poll_time(dt_status_t *status) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    status->hour = timeinfo->tm_hour;
    status->minute = timeinfo->tm_min;
    status->second = timeinfo->tm_sec;
}

void status_poll_battery(dt_status_t *status) {
    const char *fname = "/sys/class/power_supply/BAT0/capacity";
    FILE *fin = fopen(fname, "r");
    if (fin ==NULL) {
        printf("cannot get battery\n");
        status->battery = -1;
        return;
    }
    char str[4];
    fgets(str, 4, fin);
    status->battery = atoi(str);
    fclose(fin);
    char *icons[] = {"󰂎", "󰁺", "󰁻", "󰁼", "󰁽", "󰁾", "󰁿", "󰂀", "󰂁", "󰂂", "󰁹"};
    int i = (status->battery) / 10;
    i = i>=0 ? i<11 ? i : 10 : 0;
    status->battery_icon = icons[i];
}

char* read_text(const char *fname) {
    int fd = open(fname, O_RDONLY, 0);
    struct stat sb;
    fstat(fd, &sb);
    size_t size = sb.st_size;
    char *buf = malloc(size+1);
    read(fd, buf, size);
    buf[size] = '\0';
    close(fd);
    return buf;
}

void sub_src_tmp(char *buf, char *match, dt_status_t *status) {
    if (strcmp(match, "STATUS_HOUR")==0) {
        sprintf(buf, "%02d", status->hour);
    } else if (strcmp(match, "STATUS_MINUTE")==0) {
        sprintf(buf, "%02d", status->minute);
    } else if (strcmp(match, "STATUS_SECOND")==0) {
        sprintf(buf, "%02d", status->second);
    } else if (strcmp(match, "STATUS_BATTERY")==0) {
        sprintf(buf, "%02d", status->battery);
    } else if (strcmp(match, "STATUS_BATTERY_ICON")==0) {
        strncpy(buf, status->battery_icon, 4);
    } else {
        sprintf(buf, "??????????");
    }
}

int substitue_str(char *str, char *subs, dt_status_t *status) {
    char tokenbuf[200];
    char *s = strncpy(tokenbuf, subs, 200);
    // loop through each token of the substitute instruction, then make the substitution
    for(char *token=strtok_r(s,",", &s); token; token=strtok_r(NULL,",",&s)) {
        int from, n; // sections of the string to be replaced
        char replacement[100]; // replacement variable
        // should check token length
        sscanf(token, "%d %d %s", &from, &n, replacement);
        char *start = str + from;

        char buf[100]; // data from status
        sub_src_tmp(buf, replacement, status);
        memcpy(start, buf, n);
    }
    return 0;
}

char *trim(char *str) {
    if (!str) return NULL;
    char *c=str;
    for (; *c==' '; c++);
    int len = strlen(c) -1;
    for (; c[len]==' '||c[len]=='\n';len--);
    c[len+1] = '\0';
    return c;
}
