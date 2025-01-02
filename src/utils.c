#include "utils.h"


/* PangoLayout* pango_new_layout (PangoContext *ctx);
 *
 * pango_layout_get/set_
 *  width(int width); // -1 default inf
 *  wrap(PangoWrapMode wrap); // 0 PANGO_WRAP_WORD 1 PANGO_WRAP_CHAR 2  PANGO_WRAP_WORD_CHAR
 *  alignment(PangoAlignment alignment PANGO_ALIGN_LEFT/CENTER/RIGHT
 */

dt_status_t* create_status() {
    dt_status_t *status = malloc(sizeof(dt_status_t));
    return status;
}

void destroy_status(dt_status_t *status) {
    free(status);
}

void status_poll_time(dt_status_t *status) {
    time_t rawtime;
    struct tm *timeinfo;

    time(&rawtime);
    timeinfo = localtime(&rawtime);
    status->hour = timeinfo->tm_hour;
    status->minute = timeinfo->tm_min;
    status->second = timeinfo->tm_sec;
}

void status_poll_battery(dt_status_t *status) {
    const char *fname = "/sys/class/power_supply/BAT0/capacity";
    FILE *fin = fopen(fname, "r");
    if (fin ==NULL) {
        printf("cannot get battery\n");
        status->battery = -1;
        return;
    }
    char str[4];
    fgets(str, 4, fin);
    status->battery = atoi(str);
    fclose(fin);
}

void draw_textbox(cairo_t *cr, double xpos, double ypos, const char *text, const char *font, int color_RGBA) {
    //setup font
    PangoFontDescription *font_description;
    font_description = pango_font_description_from_string(font); // "Terminess Nerd Font 48"

    // layouts
    PangoLayout *layout = pango_cairo_create_layout(cr);
    pango_layout_set_font_description(layout, font_description);
    pango_layout_set_text(layout, text, -1); // 19\n47
    pango_layout_set_alignment(layout, PANGO_ALIGN_LEFT);

    // draw text
    cairo_set_source_rgba(cr, RGBA(color_RGBA)); // 0xffffff
    cairo_move_to(cr, xpos, ypos); //40.0, 80.0
    pango_cairo_show_layout(cr, layout);

    // free
    g_object_unref(layout);
    pango_font_description_free(font_description);
}
