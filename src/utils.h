#pragma once
#include <time.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pango/pangocairo.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RGBA(hex) ((hex>>24)&0xff)/255.0, ((hex>>16)&0xff)/255.0, ((hex>>8)&0xff)/255.0,((hex>>0)&0xff)/255.0

/* information */
// status object
typedef struct {
    short int hour;
    short int minute;
    short int second;
    short int battery;
    char* battery_icon;
} dt_status_t;
/* constructor
 */
dt_status_t* create_status();
/* deconstructor
 */
void destroy_status(dt_status_t *status);
/* updates the status in accordance with time
 */
void status_poll_time(dt_status_t *status);
/* updates status for battery
 */
void status_poll_battery(dt_status_t *status);

char* read_text(const char *fname);
int substitue_str(char *str, char *subs, dt_status_t *status);
char *trim(char *str);

/* drawing */
typedef struct {
    int fill_color;
    char text[100];
    char text_subs[100];
    char text_markup[100];
} DrawData;
typedef struct { uint32_t x,y,w,h; } rectangle_t;
typedef void draw_callback(cairo_t*, rectangle_t, void*);
