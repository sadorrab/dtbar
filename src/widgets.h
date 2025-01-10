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
#define LAYER_BACKGROUND 0
#define LAYER_BOTTOM 1
#define LAYER_TOP 2
#define LAYER_FOREGROUND 3

typedef struct tnode_t {
    // layout
    uint32_t minwidth, minheight;
    int content_anchor;
    uint32_t x, y, w, h; // dimensions relative to toplevel
    // drawing 
    DrawData draw_data;
    // tree
    int child_count;
    struct tnodet_t *children[MAX_CHILDREN];
} dt_container_t;
/* constructor
 */
dt_container_t *create_container();
/* destructor
 */
void destroy_container(dt_container_t *container);
/* Updates the dimensions of container to st outer containers stretch to fit content
 * and inner contaners know their locations relative to toplevel
 * PARAM container
 * PARAM x, y, w, h: buffers to store dimensions
 * RETURN: The size in bytes of this container
 */
size_t container_resize(dt_container_t *content);
/* applies the draw function to a container and its children
 */
void container_render_content(dt_container_t *container, cairo_t *cr, dt_status_t *status);


typedef struct {
    char *name;
    wl_pool_buffer_t *buf;
    dt_container_t *content;
    // position
    int anchor;
    int layer;
    uint32_t exclusive_zone;
    uint32_t off_top, off_bottom, off_left, off_right;
} dt_widget_t;
/* constructor
 */
dt_widget_t* create_widget();
/* deconstructor
 */
void destroy_widget(dt_widget_t *widget);
