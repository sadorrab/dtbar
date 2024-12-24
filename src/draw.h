#pragma once
#include <pango/pangocairo.h>
#include "wl.h"
#include "poll.h"

void draw_statusbar(wl_pool_buffer_t *buf, dt_status_t *status);

void draw_wallpaper(wl_pool_buffer_t *buf, dt_status_t *status);
