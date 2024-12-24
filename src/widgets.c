#include "widgets.h"

// widgets.c
// implementations for widgets

/* status bar */
#define FONT "Sans 12"
void draw_statusbar_text(wl_pool_buffer_t *buf, const char* time, const char *battery) {
    //setup font
    PangoFontDescription *font_description;
    font_description = pango_font_description_from_string(FONT);
    pango_font_description_set_family(font_description, "Terminess Nerd Font");

    // layouts
    PangoLayout *layout_left = pango_cairo_create_layout(buf->cairo);
    pango_layout_set_font_description(layout_left, font_description);
    pango_layout_set_text(layout_left, time, -1);
    pango_layout_set_alignment(layout_left, PANGO_ALIGN_LEFT);

    PangoLayout *layout_right = pango_cairo_create_layout(buf->cairo);
    pango_layout_set_font_description(layout_right, font_description);
    pango_layout_set_text(layout_right, battery, -1);
    pango_layout_set_alignment(layout_right, PANGO_ALIGN_RIGHT);

    // draw text
    cairo_set_source_rgb(buf->cairo, 1.0, 1.0, 1.0);
    cairo_move_to(buf->cairo, 18.0, 4.8);
    pango_cairo_show_layout(buf->cairo, layout_left);
    cairo_move_to(buf->cairo, 1320.0, 4.8);
    pango_cairo_show_layout(buf->cairo, layout_right);

    // free
    g_object_unref(layout_left);
    g_object_unref(layout_right);
    pango_font_description_free(font_description);
}

void bar_on_repeat(wl_pool_buffer_t *buf, void *args) {
    dt_status_t *status = (dt_status_t*) args;
    char timestr[24];
    char batterystr[8];
    sprintf(timestr, "%02d:%02d", status->hour, status->minute);
    sprintf(batterystr, "%3d%%", status->battery);
    // background
    cairo_set_source_rgb(buf->cairo, 0.10, 0.11, 0.17);
    cairo_rectangle(buf->cairo, 0, 0, buf->width, buf->height);
    cairo_fill(buf->cairo);

    // text
    draw_statusbar_text(buf, timestr, batterystr);

    update_surface(buf);
}


/* wallpaper */
void wall_on_start(wl_pool_buffer_t *buf, void *args) {
    dt_status_t *status = (dt_status_t*) args;
    cairo_set_source_rgb(buf->cairo, 0.40, 0.43, 0.64);
    cairo_rectangle(buf->cairo, 0, 0, buf->width, buf->height);
    cairo_fill(buf->cairo);

    update_surface(buf);
    (void)status;
}
