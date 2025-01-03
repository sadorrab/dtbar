#pragma once
#include <time.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <pango/pangocairo.h>
#include <sys/stat.h>
#include <fcntl.h>

#define RGB(hex) ((hex>>8)&0xff)/255.0, ((hex>>4)&0xff)/255.0,(hex&0xff)/255.0
#define RGBA(hex) ((hex>>24)&0xff)/255.0, ((hex>>16)&0xff)/255.0, ((hex>>8)&0xff)/255.0,((hex>>0)&0xff)/255.0

/* information */
// status object
typedef struct {
    short int hour;
    short int minute;
    short int second;
    short int battery;
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

/* drawing */
char* read_text(const char *fname);
void draw_textbox(cairo_t *cr, double xpos, double ypos, const char *text, const char *font, int color_RGBA);
