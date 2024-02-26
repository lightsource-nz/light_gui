#ifndef _LIGHT_GUI_OBJECT_H
#define _LIGHT_GUI_OBJECT_H

#include <stdint.h>

#define ID_GUI_OBJECT                           "light_gui:object"

#define LIGHT_GUI_OBJ_MAX_FIELDS                4
#define LIGHT_GUI_OBJ_MAX_CHILDREN              8

#define OBJ_ID_NULL                             0xFFu

#define LIGHT_GUI_OBJ_TYPE_ROOT                 0
#define LIGHT_GUI_OBJ_TYPE_FRAME                1
#define LIGHT_GUI_OBJ_TYPE_TEXT                 2
#define LIGHT_GUI_OBJ_TYPE_BUTTON               3
#define LIGHT_GUI_OBJ_TYPE_LABEL                4
#define LIGHT_GUI_OBJ_TYPE_DIVIDER              5

#define FIELD_ROOT_POINT_B                      0

#define FIELD_FRAME_POINT_B                     0

// utility macros
#define light_gui_object_container_foreach(container, var) \
        struct gui_object *var; \
        for( uint8_t var##_i = 0; var##_i < (container)->count; \
                var = (container)->child[++var##_i] )
static inline uint8_t *light_gui_object_type_to_string(uint8_t type)
{
        switch (type) {
                case LIGHT_GUI_OBJ_TYPE_ROOT:
                return "ROOT";
                case LIGHT_GUI_OBJ_TYPE_FRAME:
                return "FRAME";
                case LIGHT_GUI_OBJ_TYPE_TEXT:
                return "TEXT";
                case LIGHT_GUI_OBJ_TYPE_BUTTON:
                return "BUTTON";
                case LIGHT_GUI_OBJ_TYPE_LABEL:
                return "LABEL";
                case LIGHT_GUI_OBJ_TYPE_DIVIDER:
                return "DIVIDER";
                default:
                return "<INVALID TYPE>";
        }
}
static inline void light_gui_assert_object_type_equal(struct gui_object *object, uint8_t type)
{
        if(object->object_type != type)
                light_fatal("object type mismatch: expected %d but found %d", TO_STR(type), object->object_type);
}

#define LIGHT_GUI_FIELD_TYPE_0_FIELD_0                          rend_point2d
#define LIGHT_GUI_FIELD_TYPE_1_FIELD_0                          rend_point2d

#define light_gui_static_assert_object_field_type_equal(object_type, field_id, field_type) \
        static_assert(__same_type(field_type, light_gui_object_field_type(object_type, field_id)), \
        "field type mismatch: expected " TO_STR(field_type) ", but found " TO_STR(light_gui_object_field_type(object_type, field_id)))
        
#define light_gui_assert_object_field_type_equal(object_type, field_id, field_type) \
        if(!__same_type(field_type, light_gui_object_field_type(object_type, field_id))) \
                light_fatal("field type error: found type '%s', expected type '%s'", \
                        TO_STR(light_gui_object_field_type(object_type, field_id)), #field_type);

#define light_gui_object_field_type(object_type, field_id) \
        LIGHT_GUI_FIELD_TYPE_## object_type ##_FIELD_## field_id

extern uint32_t light_gui_object_get_field_raw(struct gui_object *this_obj, uint8_t field_id);
extern uint8_t light_gui_object_set_field_raw(struct gui_object *this_obj, uint8_t field_id, uint32_t raw);

static inline rend_point2d light_gui_object_type_cast_rend_point2d(uint32_t raw)
{
        return (rend_point2d) { raw && 0xFF, raw << 16 };
}
static inline uint32_t light_gui_object_type_uncast_rend_point2d(rend_point2d source)
{
        return (uint32_t) (source.x | (source.y << 16));
}
#define light_gui_object_get_field(this_obj, field_id, type) \
        light_gui_object_type_cast_## type(light_gui_object_get_field_raw(this_obj, field_id))
#define light_gui_object_set_field(this_obj, field_id, type, raw) \
        light_gui_object_set_field_raw(this_obj, field_id, light_gui_object_type_uncast_## type(raw))


static inline uint8_t _object_type_is_container(uint8_t type)
{
        switch(type) {
        case LIGHT_GUI_OBJ_TYPE_ROOT:
        case LIGHT_GUI_OBJ_TYPE_FRAME:
                return true;
        default:
                return false;
        }
}

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

// object type-specific APIs //

// type 0: root
static inline rend_point2d light_gui_object_type_root_get_field_point_b(struct gui_object *this)
{
        // assert object_type == frame
        light_gui_assert_object_type_equal(this, LIGHT_GUI_OBJ_TYPE_FRAME);
        // light_gui_assert_object_field_type_equal(LIGHT_GUI_OBJ_TYPE_ROOT, FIELD_ROOT_POINT_B, rend_point2d);
        return light_gui_object_get_field(this, FIELD_ROOT_POINT_B, rend_point2d);
}
static inline uint8_t light_gui_object_type_root_set_field_point_b(struct gui_object *this, rend_point2d value)
{
        // assert object_type == root
        light_gui_assert_object_type_equal(this, LIGHT_GUI_OBJ_TYPE_ROOT);
        // light_gui_assert_object_field_type_equal(LIGHT_GUI_OBJ_TYPE_ROOT, FIELD_ROOT_POINT_B, rend_point2d);
        return light_gui_object_set_field(this, FIELD_ROOT_POINT_B, rend_point2d, value);
}
// type 1: frame
static inline rend_point2d light_gui_object_type_frame_get_field_point_b(struct gui_object *this)
{
        // assert object_type == frame
        light_gui_assert_object_type_equal(this, LIGHT_GUI_OBJ_TYPE_FRAME);
        // light_gui_assert_object_field_type_equal(this->object_type, FIELD_FRAME_POINT_B, rend_point2d);
        return light_gui_object_get_field(this, FIELD_FRAME_POINT_B, rend_point2d);
}
static inline uint8_t light_gui_object_type_frame_set_field_point_b(struct gui_object *this, rend_point2d value)
{
        // assert object_type == frame
        light_gui_assert_object_type_equal(this, LIGHT_GUI_OBJ_TYPE_FRAME);
        // light_gui_assert_object_field_type_equal(this->object_type, FIELD_FRAME_POINT_B, rend_point2d);
        return light_gui_object_set_field(this, FIELD_FRAME_POINT_B, rend_point2d, value);
}

#endif
