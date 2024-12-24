#pragma once
#include "wl.h"
#include "draw.h"
#include "poll.h"

#define MAX_WIDGETS 4

typedef struct {
    surface_layout_t layout;
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

dt_ctl *create_widgets(void);
void destroy_widgets(dt_ctl *ctl);
