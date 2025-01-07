#include "widgets.h"

const int BACKGROUND = 0x8ec07cff;
const int BAR = 0x282828ff;
const int BLUE = 0x458586ff;
const int GREEN = 0x98971aff;

dt_container_t Time = {
    .minwidth = 90,
    .minheight = 20,
    .draw_fn = {draw_fill, draw_text},
    .draw_args[0] = (void*) &BAR,
    .child_count = 0
};

dt_container_t Battery = {
    .minwidth = 70,
    .minheight = 20,
    .draw_fn = {draw_fill, draw_text},
    .draw_args[0] = (void*) &BAR,
    .child_count = 0
};

dt_container_t Statusbar = {
    .minwidth = SCREEN_WIDTH,
    .minheight = 24,
    .content_anchor = ANCHOR_RIGHT | STACKING_HORIZONTAL,
    .draw_fn = {draw_fill, NULL},
    .draw_args[0] = (void*) &BAR,
    .child_count = 2,
    .children[0] = (struct tnodet_t*)(&Battery),
    .children[1] = (struct tnodet_t*)(&Time)
}; 

dt_container_t Wallpaper = {
    .minwidth = SCREEN_WIDTH,
    .minheight = SCREEN_HEIGHT,
    .content_anchor = ANCHOR_TOP | STACKING_VERTICAL,
    .draw_fn = {draw_fill, NULL},
    .draw_args[0] = (void*) &BACKGROUND,
    .child_count = 0
};
