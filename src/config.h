#include "wl.h"
#include "utils.h"
#include "widgets.h"
#include "main.h"

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

static int widget_count  = 2;
static dt_spec_t widget_specs[] = {
    {
        .name = "statusbar",
        .width = SCREEN_WIDTH,
        .height = 30,
        .exclusive_zone = 30,
        .layer = 1, // bottom
        .anchor = 1, // top
        .on_start = NULL,
        .on_repeat = bar_on_repeat
    },
    {
        .name = "wallpaper",
        .width = SCREEN_WIDTH,
        .height = SCREEN_HEIGHT,
        .exclusive_zone = 0,
        .layer = 0, // background
        .anchor = 1,
        .on_start = wall_on_start,
        .on_repeat = NULL
    }
};
