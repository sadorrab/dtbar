#pragma once
#include <pango/pangocairo.h>
#include "wl.h"

void draw_statusbar(wl_pool_buffer_t *buf);

void draw_wallpaper(wl_pool_buffer_t *buf);
