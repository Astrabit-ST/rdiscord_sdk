#include "rdiscord_sdk.h"
#include "common.h"
#include "gem_activity_manager.h"
#include "activity.h"
#include "user.h"
#include "gem_user_manager.h"
#include "gem_overlay_manager.h"

using namespace Rice;

VALUE rb_oPendingCallbacks;
VALUE rb_oProcEvents;

Module rb_mDiscord;
DiscordStruct DiscordSDK;

Object rb_core_initialize(long long client_id, long flags) {
    if (DiscordSDK.core) {
        rb_raise(rb_eRuntimeError, "Discord already initialized!");
    }
    discord::Core* core;
    auto result = discord::Core::Create(client_id, flags, &core);
    DiscordSDK.core.reset(core);
    return rb_result_to_obj(result);
}

void rb_core_log_hook(discord::LogLevel minLevel, const char* message) {
    if (!DiscordSDK.log_hook) {
        return;
    }
    VALUE args = rb_ary_new_from_args(2, minLevel, message);
    rb_proc_call(DiscordSDK.log_hook, args);
}

Object rb_core_set_log_hook(int loglevel) {
    CHECK_CORE_INITIALIZED;
    if (DiscordSDK.log_hook) {
        rb_hash_delete(rb_oProcEvents, Symbol("log_hook"));
    }

    DiscordSDK.log_hook = rb_common_get_event_proc(2);
    rb_common_add_proc_to_hash(Symbol("log_hook"), DiscordSDK.log_hook);
    
    DiscordSDK.core->SetLogHook(
        (discord::LogLevel) loglevel,
        rb_core_log_hook   
    );

    return Qnil;
}

Object rb_core_run_callbacks() {
    CHECK_CORE_INITIALIZED;
    return rb_result_to_obj(DiscordSDK.core->RunCallbacks());
}

extern "C"
void Init_rdiscord_sdk()
{
    rb_oPendingCallbacks = rb_ary_new();
    rb_oProcEvents = rb_hash_new();

    rb_mDiscord = define_module("Discord");
    rb_mDiscord.define_module_function("init", &rb_core_initialize);
    rb_mDiscord.define_module_function("set_log_hook", &rb_core_set_log_hook);
    rb_mDiscord.define_module_function("run_callbacks", &rb_core_run_callbacks);

    rb_global_variable(&rb_oProcEvents);
    rb_global_variable(&rb_oPendingCallbacks);

    rb_gv_set("$discord_proc_events", rb_oProcEvents);
    rb_gv_set("$discord_pending_callbacks", rb_oPendingCallbacks);

    rb_activity_init_manager(rb_mDiscord);
    rb_activity_init_class(rb_mDiscord);
    rb_user_init_class(rb_mDiscord);
    rb_user_init_manager(rb_mDiscord);
    rb_overlay_init_manager(rb_mDiscord);
}