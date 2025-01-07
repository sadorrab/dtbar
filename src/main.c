#include "main.h"

int main(void) {
    dt_status_t *status = create_status();
    dt_widget_t statusbar = {
        .content = &Statusbar,
        .layer = 1, // bottom
        .anchor = ANCHOR_TOP 
    };
    dt_widget_t wallpaper = {
        .content = &Wallpaper,
        .layer = 0, // background
        .anchor = ANCHOR_TOP 
    };

    size_t s = container_resize(statusbar.content) + container_resize(wallpaper.content);

    wl_init();
    wl_pool_ctl_t *shm = create_pool(s);
    const wl_surface_layout bar_layout = {
        .name = "statusbar",
        .width=statusbar.content->w,.height=statusbar.content->h,
        .exclusive_zone = statusbar.content->h,
        .off_right=0, .off_top=0, .off_bottom=0, .off_left = 0,
        .layer = 1,
        .anchor = ANCHOR_BOTTOM
    };
    wl_pool_buffer_t *bar_buf = create_buffer(shm, bar_layout);

    const wl_surface_layout wall_layout = {
        .name = "wallpaper",
        .width=wallpaper.content->w,.height=wallpaper.content->h,
        .exclusive_zone = 0,
        .off_right=0, .off_top=0, .off_bottom=0, .off_left = 0,
        .layer = 0,
        .anchor = ANCHOR_TOP
    };
    wl_pool_buffer_t *wall_buf = create_buffer(shm, wall_layout);
    
    do_draw_call(wall_buf->cairo, wallpaper.content, 0);
    do_draw_call(bar_buf->cairo, statusbar.content, 0);
    update_surface(bar_buf);
    update_surface(wall_buf);

    while (1) {
        char time_markup[200];
        status_poll_time(status);
        sprintf(time_markup, "<span font_desc=\"Terminess Nerd Font 12\" foreground=\"#ebdbb2\">%02d:%02d</span>", status->hour, status->minute);
        Time.draw_args[1] = (void*) time_markup;

        char battery_markup[200];
        status_poll_battery(status);
        sprintf(battery_markup, "<span font_desc=\"Terminess Nerd Font 12\" foreground=\"#ebdbb2\">%s %d%%</span>", status->battery_icon, status->battery);
        Battery.draw_args[1] = (void*) battery_markup;

        do_draw_call(bar_buf->cairo, statusbar.content, 0);
        do_draw_call(bar_buf->cairo, statusbar.content, 1);
        update_surface(bar_buf);
        update_surface(wall_buf);
        sleep(20);
    }

    destroy_status(status);
    destroy_buffer(bar_buf);
    destroy_buffer(wall_buf);
    destroy_pool(shm);
    wl_fin();

    return 0;
}
