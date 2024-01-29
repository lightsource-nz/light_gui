#include <light.h>
#include <module/mod_light_display.h>
#include <rend.h>
#include <light_gui.h>

#include "light_gui_internal.h"

static void _module_event(const struct light_module *module, uint8_t event);
Light_Module_Define(light_gui, _module_event,
                                &rend,
                                &light_display,
                                &light_framework)

static uint8_t _module_task(struct light_application *app);
static void _module_event(const struct light_module *module, uint8_t event)
{
        switch(event) {
        case LF_EVENT_LOAD:
                light_gui_init();
                light_module_register_periodic_task(&light_gui, "light_gui_task", _module_task);
                break;
                // TODO implement unregister for event hooks
         case LF_EVENT_UNLOAD:
                break; 
        }
}
static uint8_t _module_task(struct light_application *app)
{
        light_info("enter light-gui module task, time=%dms, time since last run=%dms", 0, 0);
        _light_gui_task_run();
        
        return LF_STATUS_RUN;
}
