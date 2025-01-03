#include "widgets.h"

// widgets.c
// implementations for widgets

/* wallpaper */
    
void wall_on_start(wl_pool_buffer_t *buf, void *args) {
    cairo_set_source_rgba(buf->cairo, RGBA(0x151515ff));
    cairo_rectangle(buf->cairo, 0, 0, buf->width, buf->height);
    cairo_fill(buf->cairo);

    // markuptext
    char *markup = read_text("images/arch.ti");
    char *str = malloc(1200*sizeof(char));
    PangoAttrList *attribs = pango_attr_list_new();
    pango_parse_markup(markup, -1, 0, &attribs, &str, NULL, NULL);
    PangoLayout *layout = pango_cairo_create_layout(buf->cairo);
    pango_layout_set_attributes(layout, attribs);
    pango_layout_set_text(layout, str, -1);
    cairo_move_to(buf->cairo, 800, 160);
    pango_cairo_show_layout(buf->cairo, layout);

    pango_attr_list_unref(attribs);
    g_object_unref(layout);
    free(str);
    free(markup);
    update_surface(buf);
    (void)args;
}

/* clock */
void clock_on_repeat(wl_pool_buffer_t *buf, void *args) {
    dt_status_t *status = (dt_status_t*) args;
    // clear
    cairo_set_operator(buf->cairo, CAIRO_OPERATOR_CLEAR);
    cairo_set_source_rgba(buf->cairo, RGBA(0x00000000));
    cairo_rectangle(buf->cairo, 0, 0, buf->width, buf->height);
    cairo_fill(buf->cairo);

    // text
    cairo_set_operator(buf->cairo, CAIRO_OPERATOR_SOURCE);
    status_poll_time(status);
    char markup[200];
    sprintf(markup, "<span font_desc=\"Terminess Nerd Font 120\" foreground=\"#832e31\">%02d:%02d</span>", status->hour, status->minute);
    char *str = malloc(60*sizeof(char));
    PangoAttrList *attribs = pango_attr_list_new();
    pango_parse_markup(markup, -1, 0, &attribs, &str, NULL, NULL);
    PangoLayout *layout = pango_cairo_create_layout(buf->cairo);
    pango_layout_set_attributes(layout, attribs);
    pango_layout_set_text(layout, str, -1);
    cairo_move_to(buf->cairo, 40, 0);
    pango_cairo_show_layout(buf->cairo, layout);
    
    pango_attr_list_unref(attribs);
    g_object_unref(layout);
    free(str);
    update_surface(buf);
}

dt_ctl *create_widgets(int specc, const dt_spec_t specs[]) {
    wl_init();
    dt_ctl *ctl = malloc(sizeof(dt_ctl));
    ctl->widget_count = specc;

    ctl->status = create_status();

    // get total memory
    size_t total_size = 0;
    for (int i=0; i<specc; i++) {
        int width = specs[i].layout.width;
        int height = specs[i].layout.height;
        int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, width);
        total_size += height * stride;
    }
    wl_pool_ctl_t *pool = create_pool(total_size);
    ctl->pool_ctl = pool;

    // create widgets
    for (int i=0; i<specc; i++) {
        dt_widget_t *wig = &(ctl->widgets[i]);
        wig->buf  = create_buffer(pool, specs[i].layout);
        wig->on_start = specs[i].on_start;
        wig->on_repeat = specs[i].on_repeat;
    }

    return ctl;
}

void destroy_widgets(dt_ctl *ctl) {
    for (int i=0; i<ctl->widget_count; i++) {
        destroy_buffer(ctl->widgets[i].buf);
    }
    destroy_pool(ctl->pool_ctl);
    destroy_status(ctl->status);
    free(ctl);
    wl_fin();
}
