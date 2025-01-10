#include "widgets.h"

#define TABLE_SIZE 11

const int BACKGROUND = 0x8ec07cff;
const int BAR = 0x282828ff;
const int BLUE = 0x458586ff;
const int GREEN = 0x98971aff;

dt_container_t Time = {
    .minwidth = 90,
    .minheight = 20,
    .draw_data = {
        .fill_color = BAR,
        .text = "11:23",
        .text_subs = "0 2 STATUS_HOUR, 3 2 STATUS_MINUTE",
        .text_markup = "0 -1 font-desc \"Terminess Nerd Font 12\", 0 -1 foreground #ffffff"
    },
    .child_count = 0
};

dt_container_t Battery = {
    .minwidth = 70,
    .minheight = 20,
    .draw_data = {
        .fill_color = BAR,
        .text = "󰂁 34%",
        .text_subs = "0 4 STATUS_BATTERY_ICON, 5 2 STATUS_BATTERY",
        .text_markup = "0 -1 font-desc \"Terminess Nerd Font 12\", 0 -1 foreground #ffffff"
    },
    .child_count = 0
};

dt_container_t Statusbar = {
    .minwidth = SCREEN_WIDTH,
    .minheight = 0,
    .content_anchor = ANCHOR_RIGHT | STACKING_HORIZONTAL,
    .draw_data = {
        .fill_color = BAR,
        .text[0] ='\0',
        .text_subs[0] = '\0'
    },
    .child_count = 2,
    .children[0] = (struct tnodet_t*)(&Battery),
    .children[1] = (struct tnodet_t*)(&Time)
}; 

dt_container_t Wallpaper = {
    .minwidth = SCREEN_WIDTH,
    .minheight = SCREEN_HEIGHT,
    .content_anchor = ANCHOR_TOP | STACKING_VERTICAL,
    .draw_data = {
        .fill_color = BLUE,
        .text[0] ='\0',
        .text_subs[0] = '\0'
    },
    .child_count = 0
};
