#include "utils.h"
#include "wl.h"

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768

#define MAX_WIDGETS 4

typedef void (*dt_wig_fn) (wl_pool_buffer_t*, void *args);
typedef struct {
    wl_surface_layout layout;
    dt_wig_fn on_start;
    dt_wig_fn on_repeat;
} dt_spec_t;

typedef struct {
    wl_pool_buffer_t *buf;
    dt_wig_fn on_start;
    dt_wig_fn on_repeat;
} dt_widget_t;

typedef struct {
    int widget_count;
    dt_widget_t widgets[MAX_WIDGETS];
    wl_pool_ctl_t *pool_ctl;
    dt_status_t *status;
} dt_ctl;

dt_ctl *create_widgets(int specc, const dt_spec_t specs[]);
void destroy_widgets(dt_ctl *ctl);

/* wallpaper */
void wall_on_start(wl_pool_buffer_t *buf, void *args);

/* clockk */
void clock_on_repeat(wl_pool_buffer_t *buf, void *args);
