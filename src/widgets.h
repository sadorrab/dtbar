#include "utils.h"
#include "wl.h"

#define SCREEN_WIDTH 1366
#define SCREEN_HEIGHT 768
#define MAX_CHILDREN 10
#define MAX_DRAW_FN 10
#define ANCHOR_TOP 1
#define ANCHOR_BOTTOM 2
#define ANCHOR_LEFT 4
#define ANCHOR_RIGHT 8
#define STACKING_HORIZONTAL 16
#define STACKING_VERTICAL 32

typedef struct tnode_t {
    // layout
    uint32_t minwidth, minheight;
    int content_anchor;
    uint32_t x, y, w, h; // dimensions relative to toplevel
    // drawing 
    int background_color;
    draw_callback *draw_fn[MAX_DRAW_FN];
    void *draw_args[MAX_DRAW_FN];
    // tree
    int child_count;
    struct tnodet_t *children[MAX_CHILDREN];
} dt_container_t;
/* Updates the dimensions of container to st outer containers stretch to fit content
 * and inner contaners know their locations relative to toplevel
 * PARAM container
 * PARAM x, y, w, h: buffers to store dimensions
 * RETURN: The size in bytes of this container
 */
size_t container_resize(dt_container_t *content);
/* applies a given draw callback to a container and its children
 */
void do_draw_call(cairo_t *cr, dt_container_t *container, int call_number);


typedef struct {
    wl_pool_buffer_t *buf;
    dt_container_t *content;
    // position
    int anchor;
    int layer;
    uint32_t exclusive_zone;
    uint32_t off_top, off_bottom, off_left, off_right;
} dt_widget_t;
dt_widget_t* dt_widget_init(dt_container_t *content);
