#include "main.h"
#include "config.h"

int main(void) {
    dt_status_t status;
    passive_poll_args args = {
        .status = &status,
        .period_sec = 1
    };
    pthread_t poll;
    pthread_create(&poll, NULL, periodic_poll, &args);
    dt_ctl *ctl = create_widgets();

    dt_widget_t *bar = &ctl->widgets[0];
    while (1) {
        (bar->draw_fn) (bar->buf, &status);
        update_surface(bar->buf);
        sleep(1);
    }

    pthread_kill(poll, SIGKILL);
    pthread_join(poll, NULL);
    destroy_widgets(ctl);
    return 0;
}

dt_ctl *create_widgets(void) {
    wl_init();
    dt_ctl *ctl = malloc(sizeof(dt_ctl));
    ctl->widget_count = widget_count;

    // calculate total size for pool
    size_t total_size = 0;
    for (int i=0; i<widget_count; i++) {
        total_size += specs[i].layout.width * specs[i].layout.height * PIXEL_SIZE;
    }
    ctl->pool_ctl = create_pool(total_size);

    // create widgets
    for (int i=0; i<widget_count; i++) {
        ctl->widgets[i].buf = create_buffer(ctl->pool_ctl, specs[i].layout);
        ctl->widgets[i].draw_fn = specs[i].draw_fn;
    }
    return ctl;
}

void destroy_widgets(dt_ctl *ctl) {
    for (int i=0; i<ctl->widget_count; i++) {
        destroy_buffer(ctl->widgets[i].buf);
    }
    destroy_pool(ctl->pool_ctl);
    free(ctl);
    wl_fin();
}
