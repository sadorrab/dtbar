#include "wl.h"
#include "main.h"

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

static int widget_count  = 1;
static dt_spec_t specs[] = {
    {
        .layout = {
            .name = "statusbar",
            .width = SCREEN_WIDTH,
            .height = 30,
            .exclusive_zone = 30,
            .layer = 1, // bottom
            .anchor = 1 // top
        },
        .draw_fn = draw_statusbar
    }
};
