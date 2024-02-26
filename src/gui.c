#include <light_gui.h>

#include "light_gui_internal.h"

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
void light_gui_init()
{
        layer_count = 0;
}
/*
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
*/
struct gui_layer *light_gui_layer_init(struct gui_layer *layer, const uint8_t *format, ...)
{
        va_list args;

        va_start(args, format);
        light_gui_layer_init_va(layer, format, args);
        va_end(args);
}
struct gui_layer *light_gui_layer_init_va(struct gui_layer *layer, const uint8_t *format, va_list args)
{
        light_object_init(&layer->header, &ltype_gui_layer, format, args);
}
struct gui_layer *light_gui_layer_create(const uint8_t *format, ...)
{
        va_list args;

        va_start(args, format);
        return light_gui_layer_create_va(format, args);
        va_end(args);
}
struct gui_layer *light_gui_layer_create_va(const uint8_t *format, va_list args)
{
        struct gui_layer *layer;

        if(!(layer = light_alloc(sizeof(struct gui_layer)))) {
                light_error("could not allocate new layer: out of memory","");
                return NULL;
        }
        return light_gui_layer_init_va(layer, format, args);
}

extern struct gui_layer *light_gui_layer_get(struct gui_layer *layer)
{
        return to_gui_layer(light_object_get(&layer->header));
}
extern void light_gui_layer_put(struct gui_layer *layer)
{
        light_object_put(&layer->header);
}

const uint8_t *light_gui_layer_get_name(struct gui_layer *layer)
{
        return light_object_get_name(&layer->header);
}

uint8_t light_gui_layer_add_object(struct gui_layer *layer, struct gui_object *object)
{
        if(layer->object_count >= LIGHT_GUI_LAYER_MAX_OBJECTS) {
                light_error("failed to add gui object id #%d to layer '%s': layer contains too many objects (%d)", object->object_id, light_object_get_name(&layer->header), LIGHT_GUI_LAYER_MAX_OBJECTS);
                return LIGHT_NO_RESOURCE;
        }
        uint8_t id = layer->object_count++;
        object->object_id = id;
        layer->object[id] = object;
        return LIGHT_OK;
}
uint8_t light_gui_object_add(struct gui_object *parent, struct gui_object *child)
{
        // ASSERT(parent->children)
        if(!parent->children) {
                light_error("failed to add gui object '%s' to parent object '%s': "
                                "parent object cannot contain children",
                                light_gui_object_get_name(child),
                                light_gui_object_get_name(parent));
                return LIGHT_INVALID;
        }
        // ASSERT(parent->children->count < LIGHT_GUI_OBJ_MAX_CHILDREN)
        if(!(parent->children->count < LIGHT_GUI_OBJ_MAX_CHILDREN)) {
                light_error("failed to add gui object '%s' to parent object '%s': parent object has too many children (%d)", light_gui_object_get_name(child), light_gui_object_get_name(parent), parent->children->count);
                return LIGHT_NO_RESOURCE;
        }
        uint8_t status;
        struct gui_layer *layer_ref = light_gui_object_get_layer(parent);
        if(status = light_gui_layer_add_object(layer_ref, child)) {
                light_error("failed to add gui object '%s' to layer '%s', error code 0x%x: %s",
                                light_gui_object_get_name(child), light_gui_object_get_name(parent),
                                status, light_error_to_string(status));
                return status;
        }
        light_gui_layer_put(layer_ref);
        if(status = light_object_add(&parent->header, &child->header)) {
                light_error("failed to add gui object '%s' to parent object '%s', error code 0x%x: %s",
                                light_gui_object_get_name(child), light_gui_object_get_name(parent),
                                status, light_error_to_string(status));
                return status;
        }
        parent->children->child[parent->children->count++] = child;
        light_info("added object '%s' to parent object '%s' in object graph of layer: '%s'", child->header.id, parent->header.id, parent->layer->header.id);
}
void light_gui_layer_update(struct gui_layer *layer)
{
        light_trace("name=%s", light_gui_layer_get_name(layer));
        light_gui_object_update(layer->root);
}

// called from module periodic task
void _light_gui_task_run()
{
        for(uint8_t i = 0; i < layer_count; i++) {
                struct gui_layer *this_layer = layer[i];
                light_gui_layer_update(this_layer);
        }
}
