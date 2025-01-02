#include "main.h"

int main(void) {
    dt_ctl *ctl = create_widgets(widget_count, widget_specs);
    for (int i=0; i<ctl->widget_count; i++) {
        dt_widget_t wig = ctl->widgets[i];
        if (wig.on_start != NULL) {
            wig.on_start(wig.buf, ctl->status);
        }
    }
    while (1) {
        for (int i=0; i<ctl->widget_count; i++) {
            dt_widget_t wig = ctl->widgets[i];
            if (wig.on_repeat != NULL) {
                wig.on_repeat(wig.buf, ctl->status);
            }
        }
        sleep(20);
    }
    destroy_widgets(ctl);
    return 0;
}

