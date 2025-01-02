#include "wl.h"

wl_context_t ctx;

/* LISTENERS */
// registry
void registry_global_handler (
    void *data,
    struct wl_registry *registry,
    uint32_t name,
    const char *interface,
    uint32_t version
) {
    if (strcmp(interface, "wl_compositor")==0) {
        ctx.compositor = wl_registry_bind(registry, name, &wl_compositor_interface, 6);
    } else if (strcmp(interface, "wl_shm")==0) {
        ctx.shm = wl_registry_bind(registry, name, &wl_shm_interface, 2);
    } else if (strcmp(interface, "zwlr_layer_shell_v1")==0) {
        ctx.layer_shell = wl_registry_bind(registry, name, &zwlr_layer_shell_v1_interface, 3);
    }
    (void)data;
    (void)version;
}
void registry_global_remove_handler (
    void *data,
    struct wl_registry *registry,
    uint32_t name
) {
    (void)data;
    (void)registry;
    (void)name;
}
struct wl_registry_listener registry_listener = {
    .global = registry_global_handler,
    .global_remove = registry_global_remove_handler
};

// layer surface

void layer_surface_configure_handler (
    void *data,
    struct zwlr_layer_surface_v1 *layer_surface,
    uint32_t serial,
    uint32_t width,
    uint32_t height
) {
    printf("configure to %d x %d\n", width, height);
    zwlr_layer_surface_v1_ack_configure(layer_surface, serial);
    (void)data;
}
void layer_surface_close_handler (
    void *data,
    struct zwlr_layer_surface_v1 *layer_surface
) {
    (void)data;
    (void)layer_surface;
}
struct zwlr_layer_surface_v1_listener layer_surface_listener = {
    .configure = layer_surface_configure_handler,
    .closed = layer_surface_close_handler
};

// buffer listener
void buffer_release_handler (
    void *data,
    struct wl_buffer *buffer
) {
    printf("buffer has been released\n");
    (void)(data);
    (void)(buffer);
}
struct wl_buffer_listener buffer_listener = {
    .release = buffer_release_handler
};

int wl_init(void) {
    ctx.display = wl_display_connect(NULL);
    ctx.registry = wl_display_get_registry(ctx.display);
    wl_registry_add_listener(ctx.registry, &registry_listener, NULL);

    // block until interfaces found
    wl_display_roundtrip(ctx.display);

    if (ctx.compositor==NULL||ctx.shm==NULL||ctx.layer_shell==NULL) {
        fprintf(stderr, "ERROR: missing wl globals\n");
        return 1;
    }

    return 0;
}

void wl_fin(void) {
    return;
}

wl_pool_ctl_t *create_pool(size_t size) {
    wl_pool_ctl_t *ctl = malloc(sizeof(wl_pool_ctl_t));
    ctl->fd = syscall(SYS_memfd_create, "memorypool", 0);
    ftruncate(ctl->fd, size);
    ctl->data = mmap(NULL, size, PROT_READ|PROT_WRITE,MAP_SHARED, ctl->fd, 0);
    ctl->pool = wl_shm_create_pool(ctx.shm, ctl->fd, size);
    ctl->size = size;
    ctl->offset = 0;
    return  ctl;
}

void destroy_pool(wl_pool_ctl_t *ctl) {
    wl_shm_pool_destroy(ctl->pool);
    munmap(ctl->data, ctl->size);
    free(ctl);
}

wl_pool_buffer_t *create_buffer(wl_pool_ctl_t *ctl, const wl_surface_layout layout) {
    wl_pool_buffer_t *buf = malloc(sizeof(wl_pool_buffer_t));

    buf->surface = wl_compositor_create_surface(ctx.compositor);
    buf->layer_surface = zwlr_layer_shell_v1_get_layer_surface(ctx.layer_shell, buf->surface, NULL, layout.layer, layout.name); 
    zwlr_layer_surface_v1_add_listener(buf->layer_surface, &layer_surface_listener, NULL);

    buf->width = layout.width;
    buf->height = layout.height;
    int stride = cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, layout.width);
    buf->size = layout.height * stride;

    // ask to set layer surface properties
    zwlr_layer_surface_v1_set_size(buf->layer_surface, buf->width, buf->height);
    zwlr_layer_surface_v1_set_anchor(buf->layer_surface, layout.anchor); 
    zwlr_layer_surface_v1_set_exclusive_zone(buf->layer_surface, layout.exclusive_zone);
    zwlr_layer_surface_v1_set_margin(buf->layer_surface, layout.off_top, layout.off_right, layout.off_bottom, layout.off_left);
    wl_surface_commit(buf->surface); // surface should be configured

    // create buffer
    buf->buffer = wl_shm_pool_create_buffer(ctl->pool, ctl->offset, buf->width, buf->height, stride, WL_SHM_FORMAT_ARGB8888);
    wl_buffer_add_listener(buf->buffer, &buffer_listener, NULL);
    buf->data = (void*) (ctl->data + ctl->offset);
    ctl->offset += buf->size;

    // initialize cairo and create surface for data
    buf->cairo_surface = cairo_image_surface_create_for_data(buf->data, CAIRO_FORMAT_ARGB32, buf->width, buf->height, stride);
    buf->cairo = cairo_create(buf->cairo_surface);
    return buf;
}

void destroy_buffer(wl_pool_buffer_t *pool_buf) {
    wl_buffer_destroy(pool_buf->buffer);
    zwlr_layer_surface_v1_destroy(pool_buf->layer_surface);
    wl_surface_destroy(pool_buf->surface);
    cairo_destroy(pool_buf->cairo);
    cairo_surface_destroy(pool_buf->cairo_surface);
    free(pool_buf);
}

void update_surface(wl_pool_buffer_t *buf) {
    wl_surface_attach(buf->surface, buf->buffer, 0,0); // use this buffer for content
    wl_surface_damage_buffer(buf->surface, 0,0,buf->width,buf->height); //mark where pending is different
    wl_display_roundtrip(ctx.display);
    wl_surface_commit(buf->surface);
    wl_display_dispatch(ctx.display);
}
