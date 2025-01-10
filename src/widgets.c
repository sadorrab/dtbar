#include "widgets.h"

dt_container_t *create_container() {
    dt_container_t *ct = malloc(sizeof(dt_container_t));
    ct->content_anchor = 0;
    ct->minwidth = ct->minheight = 0;
    ct->x = ct->y = ct->w = ct->h = 0;
    ct->draw_data.fill_color = 0;
    ct->draw_data.text[0] = '\0';
    ct->draw_data.text_subs[0] = '\0';
    ct->draw_data.text_markup[0] = '\0';
    return ct;
}

void destroy_container(dt_container_t *container) {
    free(container);
}

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
    return total_size;
}

void draw_content(cairo_t *cr, rectangle_t surf, DrawData *dd, dt_status_t *status) {
    if (dd->fill_color) {
        cairo_rectangle(cr, surf.x, surf.y, surf.w, surf.h);
        cairo_set_source_rgba(cr, RGBA(dd->fill_color));
        cairo_fill(cr);
    }
    if (dd->text[0]) {
        if (dd->text_subs[0]) {
            substitue_str(dd->text, dd->text_subs, status);
        }

        char *markup = dd->text_markup[0]? dd->text_markup : "0 -1 font-desc \"Sans\", 0 -1 foreground #000000";
        PangoAttrList *attribs = pango_attr_list_from_string(markup);
        PangoLayout *layout = pango_cairo_create_layout(cr);
        pango_layout_set_text(layout, dd->text, -1);
        pango_layout_set_attributes(layout, attribs);
        pango_layout_set_width(layout, surf.w * PANGO_SCALE);
        pango_layout_set_alignment(layout, PANGO_ALIGN_CENTER);
        cairo_move_to(cr, surf.x, surf.y);
        pango_cairo_show_layout(cr, layout);
        pango_attr_list_unref(attribs);
        g_object_unref(layout);

    }
}

void container_render_content(dt_container_t *container, cairo_t *cr, dt_status_t *status) {
    rectangle_t dimensions = {.x=container->x, .y=container->y, .w=container->w, .h=container->h};
    draw_content(cr, dimensions, &(container->draw_data), status);
    
    for (int i=0; i<container->child_count; i++) {
        dt_container_t *inner = (dt_container_t*) container->children[i];
        container_render_content(inner, cr, status);
    }
}

dt_widget_t* create_widget() {
    dt_widget_t *wg = malloc(sizeof(dt_widget_t));
    wg->anchor = wg->layer = 0;
    wg->off_top = wg->off_bottom =  wg->off_right = wg->off_left = 0;
    return wg;
}

void destroy_widget(dt_widget_t *widget) {
    free(widget);
}
