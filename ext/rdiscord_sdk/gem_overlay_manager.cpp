#include "gem_overlay_manager.h"
#include <rice/rice.hpp>
#include "common.h"
#include "discord.h"

using namespace Rice;

Module rb_mOverlayManager;

bool rb_overlay_manager_is_enabled() {
    CHECK_CORE_INITIALIZED;

    bool enabled;

    DiscordSDK.core->OverlayManager().IsEnabled(&enabled);

    return enabled;
}

bool rb_overlay_manager_is_locked() {
    CHECK_CORE_INITIALIZED;

    bool locked;

    DiscordSDK.core->OverlayManager().IsLocked(&locked);

    return locked;
}

void rb_overlay_manager_set_locked(bool locked) {
    CHECK_CORE_INITIALIZED;
    
    VALUE callback_proc = rb_common_get_callback_proc(1);
    DiscordSDK.core->OverlayManager().SetLocked(
        locked,
        rb_discord_callback_wrapper_basic(callback_proc)
    );
}

void rb_overlay_manager_open_activity_invite(int type) {
    CHECK_CORE_INITIALIZED;

    VALUE callback_proc = rb_common_get_callback_proc(1);
    DiscordSDK.core->OverlayManager().OpenActivityInvite(
        (discord::ActivityActionType) type,
        rb_discord_callback_wrapper_basic(callback_proc)
    );
}

void rb_activity_open_guild_invite(const char* code) {
    CHECK_CORE_INITIALIZED;

    VALUE callback_proc = rb_common_get_callback_proc(1);
    DiscordSDK.core->OverlayManager().OpenGuildInvite(
        code,
        rb_discord_callback_wrapper_basic(callback_proc)
    );
}

void rb_overlay_manager_open_voice_settings() {
    CHECK_CORE_INITIALIZED;

    VALUE callback_proc = rb_common_get_callback_proc(1);
    DiscordSDK.core->OverlayManager().OpenVoiceSettings(
        rb_discord_callback_wrapper_basic(callback_proc)
    );
}

int rb_overlay_manager_on_toggle_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc  = rb_common_get_event_proc(1);
    int token = DiscordSDK.core->OverlayManager().OnToggle.Connect(
        [event_proc](bool locked) {
            if (event_proc == Qnil)
                return;
            
            if (!rb_obj_is_kind_of(event_proc, rb_cProc))
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(event_proc));

            
            VALUE array = rb_ary_new_from_args(2, event_proc, BOOL2RB(locked));
            int state;
            rb_protect(rb_discord_call_proc, array, &state);

            LOG_ERROR_IF_STATE;
        }
    );
    rb_common_add_proc_to_hash(Symbol("on_toggle" + token), event_proc);

    return token;
}

Object rb_overlay_manager_on_toggle_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->OverlayManager().OnToggle.Disconnect(token);
    rb_hash_delete(rb_oProcEvents, Symbol("on_toggle" + token));

    return Qnil;
}

void rb_overlay_init_manager(Module module) {
    rb_mOverlayManager = define_module_under(module, "OverlayManager");
    rb_mOverlayManager.define_module_function("is_enabled", &rb_overlay_manager_is_enabled);
    rb_mOverlayManager.define_module_function("is_locked", &rb_overlay_manager_is_locked);
    rb_mOverlayManager.define_module_function("set_locked", &rb_overlay_manager_set_locked);
    rb_mOverlayManager.define_module_function("open_activity_invite", &rb_overlay_manager_open_activity_invite);
    rb_mOverlayManager.define_module_function("open_guild_invite", &rb_activity_open_guild_invite);
    rb_mOverlayManager.define_module_function("open_voice_settings", &rb_overlay_manager_open_voice_settings);
    DEF_METHOD_EVENT(rb_mOverlayManager, toggle, overlay_manager);
}