#include <light_gui.h>
#include <rend.h>

#include "light_gui_internal.h"

#define FIELD_FRAME_POINT_B                             0
#define FIELD_FRAME_PAD                                 1
#define FIELD_FRAME_POINT_B                             0

#define FIELD_TEXT_POINT_B                              0
#define FIELD_TEXT_ROWS                                 1
#define FIELD_TEXT_WRAP                                 2

static const struct gui_object_ops object_ops_table[] = {
        { _type_frame_update    },
        { _type_text_update     },
        { _type_button_update   },
        { _type_label_update    },
        { _type_divider_update  }
};

static void _object_release(struct light_object *obj)
{
        struct gui_object *gui_obj;
        if(_object_type_is_container(gui_obj->object_type)) {
                light_gui_object_container_foreach(gui_obj->children, _child) {
                        light_gui_object_put(_child);
                }
                light_free(gui_obj->children);

        }

        light_free(to_gui_object(obj));
}
static void _object_add(struct light_object *obj_child, struct light_object *obj_parent) {
        struct gui_object *child = to_gui_object(obj_child);
        struct gui_object *parent = to_gui_object(obj_parent);
        light_debug("name=%s", light_gui_object_get_name(child));
}
static void _object_add_child(struct light_object *obj_parent, struct light_object *obj_child) {
        struct gui_object *parent = to_gui_object(obj_parent);
        struct gui_object *child = to_gui_object(obj_child);
        light_debug("name=%s", light_gui_object_get_name(parent));
        struct gui_layer *layer = light_gui_object_get_layer(parent);
        if(layer != NULL) {
                light_gui_layer_add_object(layer, child);
        }
}
static const struct lobj_type ltype_gui_object = (struct lobj_type) {
        .id = ID_GUI_OBJECT,
        .release = _object_release,
        .evt_add = _object_add,
        .evt_child_add = _object_add_child
};

uint32_t light_gui_object_get_field_raw(struct gui_object *this_obj, uint8_t field_id)
{
        // ASSERT field_id < LIGHT_GUI_OBJ_MAX_FIELDS
        if(!(field_id < LIGHT_GUI_OBJ_MAX_FIELDS)) {
                light_fatal("invalid field id '%d'", field_id);
        }
        return this_obj->data[field_id];
}
uint8_t light_gui_object_set_field_raw(struct gui_object *this_obj, uint8_t field_id, uint32_t raw)
{
        
}
void _type_root_init(struct gui_object *this_obj)
{
        this_obj->children = light_alloc(sizeof(struct gui_object_container));
}
void _type_frame_init(struct gui_object *this_obj)
{
        this_obj->children = light_alloc(sizeof(struct gui_object_container));
}
void _type_text_init(struct gui_object *this_obj);
void _type_button_init(struct gui_object *this_obj);
void _type_label_init(struct gui_object *this_obj);
void _type_divider_init(struct gui_object *this_obj);

void _type_root_update(struct gui_object *this_obj)
{
        
}

void _type_frame_update(struct gui_object *this_obj)
{
        rend_point2d a, b;
        a = (rend_point2d) { this_obj->margin, this_obj->margin };
        b = light_gui_object_get_field(this_obj, FIELD_FRAME_POINT_B, rend_point2d);
        rend_draw_rect(this_obj->layer->render_ctx, a, b, false);
}
void _type_text_update(struct gui_object *this_obj)
{
        if(this_obj->updated) {
                
        }
}
void _type_button_update(struct gui_object *this_obj)
{

}
void _type_label_update(struct gui_object *this_obj)
{

}
void _type_divider_update(struct gui_object *this_obj)
{

}

struct gui_object *light_gui_object_init(
                                        struct gui_object *object,
                                        uint8_t obj_type, const uint8_t *format, ...)
{
        va_list args;

        va_start(args, format);
        light_gui_object_init_va(object, obj_type, format, args);
        va_end(args);
}
// this is where the call to light_object_init() goes if [gui_object] is managed by [light_object].
// otherwise we write directly to the name field of the [gui_object]
struct gui_object *light_gui_object_init_va(
                                        struct gui_object *object,
                                        uint8_t obj_type, const uint8_t *format, va_list args)
{
        object->object_type = obj_type;
        object->ops = &object_ops_table[obj_type];
        light_object_init(&object->header, &ltype_gui_object, format, args);

        object->ops->init(object);
        light_debug("new gui object created at 0x%H", object);
}
struct gui_object *light_gui_object_create(uint8_t obj_type, const uint8_t *format, ...)
{
        va_list args;
        va_start(args, format);
        light_gui_object_create_va(obj_type, format, args);
        va_end(args);
}
struct gui_object *light_gui_object_create_va(uint8_t obj_type, const uint8_t *format, va_list args)
{
        struct gui_object *object;

        if(!(object = light_alloc(sizeof(struct gui_object)))) {
                light_error("could not allocate new object: out of memory","");
                return NULL;
        }

        return light_gui_object_init_va(object, obj_type, format, args);
}

extern struct gui_object *light_gui_object_get(struct gui_object *object)
{
        return to_gui_object(light_object_get(&object->header));
}
extern void light_gui_object_put(struct gui_object *object)
{
        light_object_put(&object->header);
}
const uint8_t *light_gui_object_get_name(struct gui_object *object)
{
        return light_object_get_name(&object->header);
}
// TODO does this function increment reference count on the layer object? what is semantics
//      for now let's assume that it does increment, and see if it explodes
struct gui_layer *light_gui_object_get_layer(struct gui_object *object)
{
        return light_gui_layer_get(object->layer);
}
void light_gui_object_update(struct gui_object *object)
{
        // this calling order allows @object to modify the contents of its child list
        // before the children themselves are updated
        object->ops->update(object);
        if(object->children) {
                light_gui_object_container_foreach(object->children, _child) {
                        light_gui_object_update(_child);
                }
        }
}
