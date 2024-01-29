#include <light_gui.h>

#include "light_gui_internal.h"

static const struct gui_object_ops object_ops_table[] = {
        { _type_frame_update    },
        { _type_text_update     },
        { _type_button_update   },
        { _type_label_update    },
        { _type_divider_update  }
};

static uint8_t layer_count;
static struct gui_layer *layer[LIGHT_GUI_MAX_LAYERS];

static void _layer_release(struct light_object *obj)
{
        light_free(to_gui_layer(obj));
}
// event functions should log any failure as an error, but can't currently report them
// to the application. this behaviour is subject to review and change in future.
static void _layer_add(struct light_object *obj_layer, struct light_object *parent) {
        struct gui_layer *this_layer = to_gui_layer(obj_layer);
        light_debug("name=%s", this_layer->header.id);
        // perform actual init operations for new layer
        if(layer_count >= LIGHT_GUI_MAX_LAYERS) {
                light_error("could not add new layer '%s': max layers reached (%d)",
                                                        this_layer->header.id, LIGHT_GUI_MAX_LAYERS);
                return;
        }
        layer[layer_count++] = this_layer;
}
static void _layer_add_child(struct light_object *obj_layer, struct light_object *child) {
        struct gui_layer *this_layer = to_gui_layer(obj_layer);
        light_debug("name=%s", this_layer->header.id);
        // perform actual init operations for new layer
}
static const struct lobj_type ltype_gui_layer = (struct lobj_type) {
        .id = ID_GUI_LAYER,
        .release = _layer_release,
        .evt_add = _layer_add
};
static void _object_release(struct light_object *obj)
{
        light_free(to_gui_object(obj));
}
static void _object_add(struct light_object *obj_child, struct light_object *obj_parent) {
        struct gui_object *gui_obj = to_gui_object(obj_child);
        struct gui_object *layer = to_gui_object(obj_parent);
        light_debug("name=%s", gui_obj->header.id);
}
static void _object_add_child(struct light_object *obj_parent, struct light_object *obj_child) {
        struct gui_object *parent = to_gui_object(obj_parent);
        struct gui_object *child = to_gui_object(obj_child);
        light_debug("name=%s", parent->header.id);
}
static const struct lobj_type ltype_gui_object = (struct lobj_type) {
        .id = ID_GUI_OBJECT,
        .release = _object_release,
        .evt_add = _object_add,
        .evt_child_add = _object_add_child
};
void light_gui_init()
{
        layer_count = 0;
}
struct gui_layer *light_gui_layer_create()
{
        struct gui_layer *layer = light_object_alloc(sizeof(struct gui_layer));
        return light_gui_layer_init(layer);
}
struct gui_layer *light_gui_layer_init(struct gui_layer *layer)
{
        light_trace("","");
        light_object_init(&layer->header, &ltype_gui_layer);
        return layer;
}
void light_gui_object_add_child(struct gui_object *parent, struct gui_object *object, const uint8_t *id)
{
        if(parent->child_count >= LIGHT_GUI_OBJ_MAX_CHILDREN) {
                light_error("failed to add gui object '%s' to parent object '%s': parent object has too many children (%d)", id, parent->header.id, LIGHT_GUI_OBJ_MAX_CHILDREN);
                return;
        }
        uint8_t status;
        if(status = light_object_add(&parent->layer->header, &object->header, "(%s):%s", parent->layer->header.type->id, id)) {
                light_fatal("failed to add gui object '%s' to layer '%s', error code 0x%x", id, parent->layer->header.id, status);
        }
        parent->child[parent->child_count++] = object;
        light_info("added object '%s' to parent object '%s' in object graph of layer: '%s'", object->header.id, parent->header.id, parent->layer->header.id);
}
void light_gui_layer_update(struct gui_layer *layer)
{
        for(uint8_t i = 0; i < layer->object_count; i++) {
                light_gui_object_update(layer->object[i]);
        }
}

struct gui_object *light_gui_object_init(struct gui_object *object, uint8_t obj_type)
{
        object->object_type = obj_type;

        object->ops = &object_ops_table[obj_type];
}
struct gui_object *light_gui_object_create(uint8_t obj_type)
{
        return light_gui_object_init(light_alloc(sizeof(struct gui_object)), obj_type);
}
void light_gui_object_update(struct gui_object *object)
{
        object->ops->update(object);
}
// called from module periodic task
void _light_gui_task_run()
{
        for(uint8_t i = 0; i < layer_count; i++) {
                struct gui_layer *this_layer = layer[i];
                light_gui_layer_update(this_layer);
        }
}
