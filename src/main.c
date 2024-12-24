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
    
    pthread_t start_widgets;
    pthread_create(&start_widgets, NULL, dt_widget_start, &status);

    pthread_join(start_widgets, NULL);
    pthread_kill(poll, SIGKILL);
    pthread_join(poll, NULL);
    return 0;
}

dt_ctl *create_widgets(int specc, const dt_spec_t specs[]) {
    wl_init();
    dt_ctl *ctl = malloc(sizeof(dt_ctl));
    ctl->widget_count = specc;

    // calculate total size for pool
    size_t total_size = 0;
    for (int i=0; i<ctl->widget_count; i++) {
        total_size += specs[i].width * specs[i].height * PIXEL_SIZE;
    }
    ctl->pool_ctl = create_pool(total_size);

    // create widgets
    for (int i=0; i<ctl->widget_count; i++) {
        ctl->widgets[i].buf = create_buffer(ctl->pool_ctl, specs[i].name, specs[i].width, specs[i].height, specs[i].exclusive_zone, specs[i].layer, specs[i].anchor);
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

void *dt_widget_start(void *args) {
    dt_status_t *status = (dt_status_t*) args;
    dt_ctl *ctl = create_widgets(widget_count, widget_specs);
    dt_widget_t *bar = &ctl->widgets[0];
    dt_widget_t *wall = &ctl->widgets[1];
    (wall->draw_fn) (wall->buf, status);
    update_surface(wall->buf);
    while (1) {
        (bar->draw_fn) (bar->buf, status);
        update_surface(bar->buf);
        sleep(30);
    }
    destroy_widgets(ctl);
}
