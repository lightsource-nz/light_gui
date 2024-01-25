#ifndef _LIGHT_GUI_H
#define _LIGHT_GUI_H

#include <rend.h>

#include <stdint.h>

#define ID_GUI_LAYER                            "light_gui:layer"

#define LIGHT_GUI_OBJ_MAX_FIELDS                4
#define LIGHT_GUI_OBJ_MAX_CHILDREN              8
#define LIGHT_GUI_LAYER_MAX_OBJECTS             16

struct gui_layer {
        struct light_object header;
        struct rend_context *render_ctx;
        uint8_t object_count;
        struct gui_object *object[LIGHT_GUI_LAYER_MAX_OBJECTS];
};

struct gui_object {
        struct gui_layer *layer;
        struct gui_object *parent;
        uint8_t object_type;
        uint8_t child_count;
        uint32_t child[LIGHT_GUI_OBJ_MAX_CHILDREN];
        uint32_t data[LIGHT_GUI_OBJ_MAX_FIELDS];
};

#define to_gui_layer(ptr) container_of(ptr, struct gui_layer, header)

extern void light_gui_init();

#endif