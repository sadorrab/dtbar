#pragma once
#include "wl.h"
#include "draw.h"
#include "poll.h"

#define MAX_WIDGETS 4

typedef struct {
    const char name[NAME_MAX_LENGTH];
    uint32_t width, height;
    uint32_t exclusive_zone;
    int layer;
    int anchor;
    void (*draw_fn) (wl_pool_buffer_t*, dt_status_t*);
} dt_spec_t;

typedef struct {
    wl_pool_buffer_t *buf;
    void (*draw_fn) (wl_pool_buffer_t*, dt_status_t*);
} dt_widget_t;

typedef struct {
    int widget_count;
    dt_widget_t widgets[MAX_WIDGETS];
    wl_pool_ctl_t *pool_ctl;
} dt_ctl;

dt_ctl *create_widgets(int specc, const dt_spec_t specs[]);
void destroy_widgets(dt_ctl *ctl);

void *dt_widget_start(void *args);
