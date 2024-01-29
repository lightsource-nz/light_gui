#ifndef _LIGHT_GUI_H
#define _LIGHT_GUI_H

#include <rend.h>

#include <stdint.h>

#define ID_GUI_LAYER                            "light_gui:layer"
#define ID_GUI_OBJECT                           "light_gui:object"

#define LIGHT_GUI_MAX_LAYERS                    4
#define LIGHT_GUI_LAYER_MAX_OBJECTS             16
#define LIGHT_GUI_OBJ_MAX_FIELDS                4
#define LIGHT_GUI_OBJ_MAX_CHILDREN              8

#define LIGHT_GUI_OBJ_TYPE_FRAME                0
#define LIGHT_GUI_OBJ_TYPE_TEXT                 1
#define LIGHT_GUI_OBJ_TYPE_BUTTON               2
#define LIGHT_GUI_OBJ_TYPE_LABEL                3
#define LIGHT_GUI_OBJ_TYPE_DIVIDER              4


struct gui_layer {
        struct light_object header;
        struct rend_context *render_ctx;
        uint8_t object_count;
        struct gui_object *object[LIGHT_GUI_LAYER_MAX_OBJECTS];
};

struct gui_object_ops;
struct gui_object {
        struct light_object header;
        const struct gui_object_ops *ops;
        volatile uint8_t updated;
        struct gui_layer *layer;
        struct gui_object *parent;
        uint8_t object_type;
        uint8_t child_count;
        struct gui_object *child[LIGHT_GUI_OBJ_MAX_CHILDREN];
        uint32_t data[LIGHT_GUI_OBJ_MAX_FIELDS];
};
struct gui_object_ops {
        void (*update)(struct gui_object *this_obj);
};

#define to_gui_layer(ptr) container_of(ptr, struct gui_layer, header)
#define to_gui_object(ptr) container_of(ptr, struct gui_object, header)

extern void light_gui_init();

extern struct gui_layer *light_gui_layer_init(struct gui_layer *layer);
extern struct gui_layer *light_gui_layer_create();
extern void light_gui_layer_add_object(struct gui_layer *layer, struct gui_object *parent, struct gui_object *object);
extern void light_gui_layer_update(struct gui_layer *layer);

extern struct gui_object *light_gui_object_init(struct gui_object *object, uint8_t obj_type);
extern struct gui_object *light_gui_object_create(uint8_t obj_type);
extern void light_gui_object_update(struct gui_object *object);

#endif