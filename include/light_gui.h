#ifndef _LIGHT_GUI_H
#define _LIGHT_GUI_H

#include <rend.h>

#include <stdint.h>

#define ID_GUI_LAYER                            "light_gui:layer"

#define LIGHT_GUI_MAX_LAYERS                    4
#define LIGHT_GUI_LAYER_MAX_OBJECTS             16
#define LIGHT_GUI_OBJ_MAX_FIELDS                4
#define LIGHT_GUI_OBJ_MAX_CHILDREN              8

struct gui_layer {
        struct light_object header;
        struct rend_context *render_ctx;
        struct gui_object *root;
        uint8_t object_count;
        struct gui_object *object[LIGHT_GUI_LAYER_MAX_OBJECTS];
};
struct gui_object_ops;
struct gui_object_container;
struct gui_object {
        uint8_t object_id;
        struct light_object header;
        const struct gui_object_ops *ops;
        uint16_t margin;
        uint16_t pad;
        volatile uint8_t updated;
        struct gui_layer *layer;
        struct gui_object *parent;
        uint8_t object_type;
        struct gui_object_container *children;
        uint32_t data[LIGHT_GUI_OBJ_MAX_FIELDS];
};
struct gui_object_ops {
        void (*init)(struct gui_object *this_obj);
        void (*update)(struct gui_object *this_obj);
};
struct gui_object_container {
        uint8_t count;
        struct gui_object *child[LIGHT_GUI_OBJ_MAX_CHILDREN];
};

#define to_gui_layer(ptr) container_of(ptr, struct gui_layer, header)
#define to_gui_object(ptr) container_of(ptr, struct gui_object, header)

#include <light_gui_object.h>

extern void light_gui_init();

extern struct gui_layer *light_gui_layer_init(struct gui_layer *layer, const uint8_t *format, ...);
extern struct gui_layer *light_gui_layer_init_va(struct gui_layer *layer, const uint8_t *format, va_list args);
extern struct gui_layer *light_gui_layer_create(const uint8_t *format, ...);
extern struct gui_layer *light_gui_layer_create_va(const uint8_t *format, va_list args);

extern struct gui_layer *light_gui_layer_get(struct gui_layer *layer);
extern void light_gui_layer_put(struct gui_layer *layer);
extern const uint8_t *light_gui_layer_get_name(struct gui_layer *layer);
// increments reference count on *object
extern uint8_t light_gui_layer_add_object(struct gui_layer *layer, struct gui_object *object);
extern void light_gui_layer_update(struct gui_layer *layer);

extern struct gui_object *light_gui_object_init(struct gui_object *object, uint8_t obj_type, const uint8_t *format, ...);
extern struct gui_object *light_gui_object_init_va(struct gui_object *object, uint8_t obj_type, const uint8_t *format, va_list args);
extern struct gui_object *light_gui_object_create(uint8_t obj_type, const uint8_t *format, ...);
extern struct gui_object *light_gui_object_create_va(uint8_t obj_type, const uint8_t *format, va_list args);

extern struct gui_object *light_gui_object_get(struct gui_object *object);
extern void light_gui_object_put(struct gui_object *object);
extern const uint8_t *light_gui_object_get_name(struct gui_object *object);
extern struct gui_layer *light_gui_object_get_layer(struct gui_object *object);
extern uint8_t light_gui_object_add(struct gui_object *parent, struct gui_object *child);
extern void light_gui_object_update(struct gui_object *object);

#endif
