#include "draw.h"
#define FONT "Sans 12"

void draw_statusbar(wl_pool_buffer_t *buf) {
    // setup pangolaout wih font and text
    PangoLayout *layout;
    PangoFontDescription *font_description;

    font_description = pango_font_description_from_string(FONT);
    pango_font_description_set_family(font_description, "Terminess Nerd Font");

    layout = pango_cairo_create_layout(buf->cairo);
    pango_layout_set_font_description(layout, font_description);
    pango_layout_set_text(layout, "Dec. 14 2022 18:33", -1);

    // drawing
    // background
    cairo_set_source_rgb(buf->cairo, 0.10, 0.11, 0.17);
    cairo_rectangle(buf->cairo, 0, 0, buf->width, buf->height);
    cairo_fill(buf->cairo);
    // text
    cairo_set_source_rgb(buf->cairo, 1.0, 1.0, 1.0);
    cairo_move_to(buf->cairo, 18.0, 4.8);
    pango_cairo_show_layout(buf->cairo, layout);

    // free
    g_object_unref(layout);
    pango_font_description_free(font_description);
}

void draw_wallpaper(wl_pool_buffer_t *buf) {
    cairo_set_source_rgb(buf->cairo, 0.23, 0.45, 0.20);
    cairo_rectangle(buf->cairo, 0, 0, buf->width, buf->height);
    cairo_fill(buf->cairo);
}
