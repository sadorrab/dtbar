#include "widgets.h"

#define ANCHOR_TOP 1
#define ANCHOR_BOTTOM 2
#define ANCHOR_LEFT 4

const int widget_count  = 2;
const dt_spec_t widget_specs[] = {
    {
        .layout = {
            .name = "wallpaper",
            .width = SCREEN_WIDTH,
            .height = SCREEN_HEIGHT,
            .exclusive_zone = 0,
            .layer = 0, // background
            .anchor = ANCHOR_TOP,
            .off_top = 0,
            .off_bottom = 0,
            .off_right = 0,
            .off_left = 0
        },
        .on_start = wall_on_start,
        .on_repeat = NULL
    },
    {
        .layout = {
            .name = "clock",
            .width = 600, 
            .height = 200,
            .layer = 1,
            .anchor = ANCHOR_BOTTOM | ANCHOR_LEFT,
            .off_top = 0,
            .off_bottom = 0,
            .off_left = 40,
            .off_right = 0
        },
        .on_start = NULL,
        .on_repeat = clock_on_repeat
    }
};
