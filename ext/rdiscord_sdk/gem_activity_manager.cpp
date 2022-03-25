#include "gem_activity_manager.h"
#include "common.h"
#include "activity.h"
#include <discord.h>

using namespace Rice;

Module rb_mActivityManager;

Object rb_activity_manager_register_command(const char* command) {
    CHECK_CORE_INITIALIZED;
    return rb_result_to_obj(
            DiscordSDK.core->ActivityManager().RegisterCommand(command)
        );
}

Object rb_activity_manager_register_steam(int steam_id) {
    CHECK_CORE_INITIALIZED;
    return rb_result_to_obj(
        DiscordSDK.core->ActivityManager().RegisterSteam(steam_id)
    );
}

Object rb_discord_manager_update_activity(Object activity) {
    CHECK_CORE_INITIALIZED;
    Object callback = rb_common_get_proc(1);
    GUARD_EXC(
        GET_ACTIVITY(activity);
    );
    return Qnil;
}

void rb_activity_init_manager(Module module) {
    rb_mActivityManager = define_module_under(module, "ActivityManager");
    rb_mActivityManager.define_module_function(
            "register_command", 
            &rb_activity_manager_register_command
        );
    rb_mActivityManager.define_module_function(
            "register_steam",
            &rb_activity_manager_register_steam
        );
}