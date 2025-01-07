#include "widgets.h"

/* CREATING WIDGETS
 * 1. determine size of the contents
 * 2. create shared memory pool
 * 3. give buffers to widgets
 */

size_t update_size(dt_container_t *content) {
    if (content->child_count == 0) {
        content->w = content->minwidth;
        content->h = content->minheight;
        return content->w * cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, content->w);
        
    } 
    // total width is sum of member widths
    // max width is width of widest member
    uint32_t totalwidth=0, totalheight=0, maxwidth=0, maxheight=0;
    for (int i=0; i<content->child_count; i++) {
        dt_container_t *inner = (dt_container_t*) content->children[i];
        update_size(inner);
        totalwidth += inner->w;
        totalheight += inner->h;
        maxwidth = inner->w>maxwidth ? inner->w : maxwidth;
        maxheight = inner->h>maxheight ? inner->h : maxheight;
    }
    // size of inner content depending wheather lined up vertically or horizontally
    if (content->content_anchor & STACKING_HORIZONTAL) {
        content->w = totalwidth>content->minwidth ? totalwidth : content->minwidth;
        content->h = maxheight>content->minheight ? maxheight : content->minheight;
    } else {
        content->w = maxwidth>content->minwidth ? maxwidth : content->minwidth;
        content->h = totalheight>content->minheight ? totalheight : content->minheight;
    }
    return content->w * cairo_format_stride_for_width(CAIRO_FORMAT_ARGB32, content->w);
}

void update_position(dt_container_t *content) {
    // origin for the inner content
    uint32_t in_x, in_y;
    // size of the inner content
    uint32_t in_w=0, in_h=0;
    for (int i=0; i<content->child_count; i++) {
        dt_container_t *inner = (dt_container_t*) content->children[i];
        if (content->content_anchor & STACKING_HORIZONTAL) {
            in_w += inner->w;
            in_h = inner->h > in_h ? inner->h : in_h;
        } else {
            in_w = inner->w > in_w ? inner->w : in_w;
            in_h += inner->h;
        }
    }
    if (content->content_anchor & ANCHOR_TOP) {
        in_y = content->y;
    } else if (content->content_anchor & ANCHOR_BOTTOM) {
        in_y = content->y + content->h - in_h;
    } else { // anchor center
        in_y = content->y + (content->h - in_h)/2;
    }
    if (content->content_anchor & ANCHOR_LEFT) {
        in_x = content->x;
    } else if (content->content_anchor & ANCHOR_RIGHT) {
        in_x = content->x + content->w - in_w;
    } else {
        in_x = content->x + (content->w - in_w)/2;
    }

    // adjust position for children
    for (int i=0; i<content->child_count; i++) {
        dt_container_t *inner = (dt_container_t*) content->children[i];
        inner->x = in_x;
        inner->y = in_y;
        if (content->content_anchor & STACKING_HORIZONTAL) {
            in_x += inner->w;
        } else if (content->content_anchor & STACKING_VERTICAL) {
            in_y += inner->h;
        }
        // children now position thier content
        update_position(inner);
    }
}

size_t container_resize(dt_container_t *content) {
    size_t total_size = update_size(content);
    // make root contianer buffer origin
    content->x = 0;
    content->y = 0;
    update_position(content);
    printf("W: %d H: %d (%d, %d)\n", content->w,content->h,content->x,content->y);
    return total_size;
}

void do_draw_call(cairo_t *cr, dt_container_t *container, int call_number) {
    draw_callback *draw = container->draw_fn[call_number];
    if (draw != NULL) {
        rectangle_t dimensions = {.x=container->x, .y=container->y, .w=container->w, .h=container->h};
        draw(cr, dimensions, (void*)container->draw_args[call_number]);
    }
    
    for (int i=0; i<container->child_count; i++) {
        dt_container_t *inner = (dt_container_t*) container->children[i];
        do_draw_call(cr, inner, call_number);
    }
}

