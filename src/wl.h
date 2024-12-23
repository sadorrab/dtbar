#pragma once
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wayland-client.h>
#include "wlr-layer-shell-protocol.h"

#include <syscall.h>
#include <unistd.h>
#include <sys/mman.h>

#include <pango/pangocairo.h>

#define PIXEL_SIZE 4
#define NAME_MAX_LENGTH 100

typedef struct {
    struct wl_compositor *compositor;
    struct wl_shm *shm;
    struct zwlr_layer_shell_v1 *layer_shell;
    struct wl_display *display;
    struct wl_registry *registry;
} wl_context_t;

typedef struct {
    struct wl_surface *surface;
    struct zwlr_layer_surface_v1 *layer_surface;
    struct wl_buffer *buffer;
    cairo_surface_t *cairo_surface;
    cairo_t *cairo;
    uint32_t width, height;
    void *data;
    size_t size;
    char avail;
} wl_pool_buffer_t;

typedef struct {
    struct wl_shm_pool *pool;
    int fd;
    void *data;
    unsigned long offset;
    size_t size;
} wl_pool_ctl_t;

typedef struct {
    const char name[NAME_MAX_LENGTH];
    uint32_t width, height;
    uint32_t exclusive_zone;
    int layer;
    int anchor;
} surface_layout_t;

int wl_init(void);
void wl_fin(void);

wl_pool_ctl_t *create_pool(size_t size);
void destroy_pool(wl_pool_ctl_t *ctl);

wl_pool_buffer_t *create_buffer(wl_pool_ctl_t *ctl, const surface_layout_t spec);
void destroy_buffer(wl_pool_buffer_t *pool_buf);

void update_surface(wl_pool_buffer_t *buf);
