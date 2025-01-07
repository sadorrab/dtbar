#include "utils.h"

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
    char *icons[] = {"󰂎", "󰁺", "󰁻", "󰁼", "󰁽", "󰁾", "󰁿", "󰂀", "󰂁", "󰂂", "󰁹"};
    int i = (status->battery) / 10;
    i = i>=0 ? i<11 ? i : 10 : 0;
    status->battery_icon = icons[i];
}

char* read_text(const char *fname) {
    int fd = open(fname, O_RDONLY, 0);
    struct stat sb;
    fstat(fd, &sb);
    size_t size = sb.st_size;
    char *buf = malloc(size+1);
    read(fd, buf, size);
    buf[size] = '\0';
    close(fd);
    return buf;
}
    
void draw_fill(cairo_t *cr, rectangle_t surf, void *args) {
    int color = *((int*) args);
    cairo_rectangle(cr, surf.x, surf.y, surf.w, surf.h);
    cairo_set_source_rgba(cr, RGBA(color));
    cairo_fill(cr);
}

void draw_text(cairo_t *cr, rectangle_t surf, void *args) {
    char *markup = (char*) args;
    int len = strlen(markup);
    char *str = malloc(len * sizeof(char));
    PangoAttrList *attribs = pango_attr_list_new();
    pango_parse_markup(markup, -1, 0, &attribs, &str, NULL, NULL);
    PangoLayout *layout = pango_cairo_create_layout(cr);
    pango_layout_set_attributes(layout, attribs);
    pango_layout_set_text(layout, str, -1);
    cairo_move_to(cr, surf.x, surf.y);
    pango_layout_set_width(layout, surf.w * PANGO_SCALE);
    pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
    pango_cairo_show_layout(cr, layout);

    pango_attr_list_unref(attribs);
    g_object_unref(layout);
    free(str);
}
