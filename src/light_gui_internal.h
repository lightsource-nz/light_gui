#ifndef _LIGHT_GUI_INTERNAL_H
#define _LIGHT_GUI_INTERNAL_H



extern void _light_gui_task_run();

// object type operations
extern void _type_root_init(struct gui_object *this_obj);
extern void _type_frame_init(struct gui_object *this_obj);
extern void _type_text_init(struct gui_object *this_obj);
extern void _type_button_init(struct gui_object *this_obj);
extern void _type_label_init(struct gui_object *this_obj);
extern void _type_divider_init(struct gui_object *this_obj);

extern void _type_root_update(struct gui_object *this_obj);
extern void _type_frame_update(struct gui_object *this_obj);
extern void _type_text_update(struct gui_object *this_obj);
extern void _type_button_update(struct gui_object *this_obj);
extern void _type_label_update(struct gui_object *this_obj);
extern void _type_divider_update(struct gui_object *this_obj);

#endif
