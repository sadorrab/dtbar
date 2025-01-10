#include "main.h"

typedef struct node_t {
    char *key;
    void *value;
    struct node_t *next;
} hashnode_t;

typedef hashnode_t *hashtable_t[TABLE_SIZE];

uint32_t hash(char *str) {
    uint32_t n = 0;
    for (char *c=str; *c!='\0'; c++) {
        n += 131 * (uint32_t) *c;
        n <<= 1;
    }
    return n;
}

hashtable_t *create_hashtable() {
    hashtable_t *table = malloc(sizeof(hashtable_t));
    for (int i=0; i<TABLE_SIZE; i++) {
        (*table)[i] = NULL;
    }
    return table;
}

int hashtable_add(hashtable_t *table, char *key, void* val) {
    hashnode_t *node = malloc(sizeof(hashnode_t));
    node->key = strdup(key);
    node->value = val;
    int i = hash(key) % TABLE_SIZE;
    node->next = (*table)[i];
    (*table)[i] = node;

    return 0;
}

void *hashtable_get(hashtable_t *table, char *key) {
    int i = hash(key) % TABLE_SIZE;
    hashnode_t *node = (*table)[i];
    while (node != NULL) {
        if (strcmp(key,node->key)==0) {
            return node->value;
        }
        node = node->next;
    }
    return NULL;
}

void hashtable_print_keys(hashtable_t *table) {
    for (int i=0; i<TABLE_SIZE; i++) {
        hashnode_t *node = (*table)[i];
        if (node == NULL) {
            continue;
        }
        printf("key: %s\n", node->key);
        while (node->next != NULL) {
            node = node->next;
            printf("key: %s\n", node->key);
        }
    }
}

dt_widget_t **read_config(hashtable_t *wig_table) {
    char *home = getenv("HOME");
    char *fname = ".config/dtbar/dtbar.conf";
    if (home==NULL) {
        printf("ERROR: no home directory\n");
        exit(0);
    }
    chdir(home);
    FILE *fin = fopen(fname, "r");
    if (!fin) {
        printf("ERROR: ' %s/%s' does not exist\n", home, fname);
        printf("copy example.conf to this file\n");
        exit(0);
    }
    char widget_names[4][100];
    int widget_count =0;
    char buf[200];
    for (int i=0; fgets(buf, 200, fin) != NULL; i++) {
        dt_container_t *current_container;
        dt_widget_t *current_widget;
        char *p=buf;
        char *prop = trim(strtok_r(buf, "=", &p));
        char *value = trim(strtok_r(NULL, "=", &p));
        if (!prop || !value) {
            continue;
        }
        // parse config file
        // terrible control flow, might or might not fix
        if (0) {
        } else if (strcmp("module", prop)==0) {
            current_container = create_container();
            hashtable_add(wig_table, value, current_container);
        } else if (strcmp("widget", prop)==0) {
            current_widget = create_widget();
            hashtable_add(wig_table, value, current_widget);
            current_widget->name = strdup(value);
            strcpy(widget_names[widget_count++], value);
        } else if (strcmp("minwidth",prop)==0) {
            current_container->minwidth = (strcmp(value,"SCREEN_WIDTH")==0)?SCREEN_WIDTH:atoi(value);
        } else if (strcmp("minheight",prop)==0) {
            current_container->minheight = (strcmp(value,"SCREEN_HEIGHT")==0)?SCREEN_HEIGHT:atoi(value);
        } else if (strcmp("content_anchor",prop)==0) {
            current_container->content_anchor |=
                (strcmp(value,"ANCHOR_TOP")==0)?ANCHOR_TOP:
                (strcmp(value,"ANCHOR_BOTTOM")==0)?ANCHOR_BOTTOM:
                (strcmp(value,"ANCHOR_LEFT")==0)?ANCHOR_LEFT:
                (strcmp(value,"ANCHOR_RIGHT")==0)?ANCHOR_RIGHT:0;
        } else if (strcmp("content_stacking",prop)==0) {
            current_container->content_anchor |=
                (strcmp(value,"STACKING_VERTICAL")==0)?STACKING_VERTICAL:
                (strcmp(value,"STACKING_HORIZONTAL")==0)?STACKING_HORIZONTAL:0;
        } else if (strcmp("fill_color",prop)==0) {
            current_container->draw_data.fill_color = strtol(value, NULL, 0x10);
        } else if (strcmp("text",prop)==0) {
            strcpy(current_container->draw_data.text, value);
        } else if (strcmp("text_subs",prop)==0) {
            strcpy(current_container->draw_data.text_subs, value);
        } else if (strcmp("text_markup",prop)==0) {
            strcpy(current_container->draw_data.text_markup, value);
        } else if (strcmp("child",prop)==0) {
            void *p = hashtable_get(wig_table, value);
            if (p) {
                struct tnodet_t *child = (struct tnodet_t*) p;
                current_container->children[current_container->child_count++] = child;
            } else {
                printf("no module \'%s\'\n", value);
            }
        } else if (strcmp("content",prop)==0) {
            void *p = hashtable_get(wig_table, value);
            if (p) {
                dt_container_t *content = (dt_container_t*) p;
                current_widget->content = content;
            } else {
                printf("no module \'%s\'\n", value);
            }
        } else if (strcmp("layer",prop)==0) {
            current_widget->layer |=
                (strcmp(value,"LAYER_BACKGROUND")==0)?LAYER_BACKGROUND:
                (strcmp(value,"LAYER_BOTTOM")==0)?LAYER_BOTTOM:
                (strcmp(value,"LAYER_TOP")==0)?LAYER_TOP:
                (strcmp(value,"LAYER_FOREGROUND")==0)?LAYER_FOREGROUND:0;
        } else if (strcmp("anchor",prop)==0) {
            current_widget->anchor |=
                (strcmp(value,"ANCHOR_TOP")==0)?ANCHOR_TOP:
                (strcmp(value,"ANCHOR_BOTTOM")==0)?ANCHOR_BOTTOM:
                (strcmp(value,"ANCHOR_LEFT")==0)?ANCHOR_LEFT:
                (strcmp(value,"ANCHOR_RIGHT")==0)?ANCHOR_RIGHT:0;
        } else if (strcmp("exclusive",prop)==0) {
            current_widget->exclusive_zone = 1;
        }
    }

    // return the widgets
    dt_widget_t **widgets = malloc(sizeof(dt_widget_t*)*(widget_count+1));
    for (int i=0; i<widget_count; i++) {
        void *p = hashtable_get(wig_table, widget_names[i]);
        if (!p) {
            fprintf(stderr, "ERROR: could not dereference \'%s\'\n", widget_names[i]);
        }
       *(widgets+i) = (dt_widget_t*) p;
    }
    *(widgets+widget_count) = NULL;

    fclose(fin);
    return widgets;
}


int main(void) {
    dt_status_t *status = create_status();
    hashtable_t *table = create_hashtable();
    dt_widget_t **widgets = read_config(table);
    // itterate through widgets to get total size needed
    wl_init();
    int wc=0;
    size_t total_bytes = 0;
    for (dt_widget_t *w=widgets[0]; w; w=widgets[++wc]) {
        total_bytes += container_resize(w->content);
    }
    wl_pool_ctl_t *shm = create_pool(total_bytes);

    // assign buffers from the shm to the widgets
    for (int i=0; i<wc; i++) {
        dt_widget_t *widget = widgets[i];
        wl_surface_layout layout = {
            .name = widget->name,
            .width = widget->content->w,
            .height = widget->content->h,
            .anchor = widget->anchor,
            .layer = widget->layer,
            .off_top = widget->off_top,
            .off_bottom = widget->off_bottom,
            .off_left = widget->off_left,
            .off_right = widget->off_right
        };
        layout.exclusive_zone =  widget->exclusive_zone?(widget->anchor&(ANCHOR_TOP|ANCHOR_BOTTOM)?layout.height:layout.width):0;
        wl_pool_buffer_t *buf =create_buffer(shm, layout);
        widget->buf = buf;
    }
    
    // draw loop
    for(;;) {
        status_poll_battery(status);
        status_poll_time(status);
        for (int i=0; i<wc; i++) {
            dt_widget_t *widget = widgets[i];
            container_render_content(widget->content, widget->buf->cairo, status);
            update_surface(widget->buf);
        }
        sleep(20);
    }
    wl_fin();

    return 0;
}
