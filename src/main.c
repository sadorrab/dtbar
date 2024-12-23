#include "wl.h"
#include "draw.h"

#define ANCHOR_TOP 1
#define LAYER_BG 0
#define LAYER_BOT 1

int main(void) {
    surface_layout_t bar_layout = {
        .name = "statusbar",
        .width = 1366,
        .height = 30,
        .exclusive_zone = 30,
        .anchor = ANCHOR_TOP, 
        .layer = LAYER_BOT
    };
    surface_layout_t bg_layout = {
        .name = "background",
        .width = 1366,
        .height = 768,
        .exclusive_zone = 0,
        .anchor = ANCHOR_TOP,
        .layer = LAYER_BG
    };
    size_t size = bar_layout.width * bar_layout.height * PIXEL_SIZE +
        bg_layout.width * bg_layout.height * PIXEL_SIZE;

    wl_init();

    wl_pool_ctl_t *pool = create_pool(size);
    wl_pool_buffer_t *bar = create_buffer(pool, bar_layout);
    wl_pool_buffer_t *wall = create_buffer(pool, bg_layout);

    draw_statusbar(bar);
    draw_wallpaper(wall);
    update_surface(bar);
    update_surface(wall);
    sleep(30);

    destroy_buffer(bar);
    destroy_buffer(wall);
    destroy_pool(pool);
    wl_fin();
    return 0;
}
