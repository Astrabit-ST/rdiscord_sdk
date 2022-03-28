#include "gem_activity_manager.h"
#include "common.h"
#include "activity.h"
#include "user.h"
#include "discord.h"

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

Object rb_activity_manager_update_activity(Object rb_activity) {
    CHECK_CORE_INITIALIZED;
    VALUE callback_proc = rb_common_get_callback_proc(1);
    
    auto fn = rb_discord_callback_wrapper_basic(callback_proc);

    GET_ACTIVITY(rb_activity);
    DiscordSDK.core->ActivityManager().UpdateActivity(
        *activity,
        fn
    );

    return Qnil;
}

Object rb_activity_manager_clear_activity() {
    CHECK_CORE_INITIALIZED;
    VALUE callback_proc = rb_common_get_callback_proc(1);

    auto fn = rb_discord_callback_wrapper_basic(callback_proc);
    DiscordSDK.core->ActivityManager().ClearActivity(fn);

    return Qnil;
}

Object rb_activity_manager_send_request_reply(std::int64_t user_id, int reply) {
    CHECK_CORE_INITIALIZED;
    VALUE callback_proc = rb_common_get_callback_proc(1);

    auto fn = rb_discord_callback_wrapper_basic(callback_proc);

    DiscordSDK.core->ActivityManager().SendRequestReply(
        user_id,
        (discord::ActivityJoinRequestReply) reply,
        fn
    );

    return Qnil;
}

Object rb_activity_manager_send_invite(std::int64_t user_id, int type, const char* content) {
    CHECK_CORE_INITIALIZED;
    VALUE callback_proc = rb_common_get_callback_proc(1);

    auto fn = rb_discord_callback_wrapper_basic(callback_proc);

    DiscordSDK.core->ActivityManager().SendInvite(
        user_id,
        (discord::ActivityActionType) type,
        content,
        fn
    );

    return Qnil;
}

Object rb_activity_manager_accept_invite(std::int64_t user_id) {
    CHECK_CORE_INITIALIZED;
    VALUE callback_proc = rb_common_get_callback_proc(1);

    auto fn = rb_discord_callback_wrapper_basic(callback_proc);

    DiscordSDK.core->ActivityManager().AcceptInvite(
        user_id,
        fn
    );

    return Qnil;
}

int rb_activity_manager_on_activity_join_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc;
    int token = DiscordSDK.core->ActivityManager().OnActivityJoin.Connect(
        [event_proc](const char* secret) {
            if (event_proc == Qnil)
                return;

            if (!rb_obj_is_kind_of(event_proc, rb_cProc))
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(event_proc));

            VALUE array = rb_ary_new_from_args(2, event_proc, rb_str_new_cstr(secret));
            int state;
            rb_protect(rb_discord_call_proc, array, &state);

            LOG_ERROR_IF_STATE;
        }
    );
    event_proc = rb_common_get_event_proc(1, Symbol("on_activity_join" + token));

    return token;
}

Object rb_activity_manager_on_activity_join_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->ActivityManager().OnActivityJoin.Disconnect(token);
    rb_hash_delete(rb_oProcEvents, Symbol("on_activity_join" + token)); // Delete event proc, we use hashes instead of arrays to delete specific entries

    return Qnil;
}

int rb_activity_manager_on_activity_spectate_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc;
    int token = DiscordSDK.core->ActivityManager().OnActivitySpectate.Connect(
        [event_proc](const char* secret) {
            if (event_proc == Qnil)
                return;

            if (!rb_obj_is_kind_of(event_proc, rb_cProc))
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(event_proc));

            VALUE array = rb_ary_new_from_args(2, event_proc, rb_str_new_cstr(secret));
            int state;
            rb_protect(rb_discord_call_proc, array, &state);

            LOG_ERROR_IF_STATE;
        }
    );
    event_proc = rb_common_get_event_proc(1, Symbol("on_activity_spectate" + token));

    return token;
}

Object rb_activity_manager_on_activity_spectate_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->ActivityManager().OnActivitySpectate.Disconnect(token);
    rb_hash_delete(rb_oProcEvents, Symbol("on_activity_spectate" + token));

    return Qnil;
}

int rb_activity_manager_on_activity_join_request_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc;
    int token = DiscordSDK.core->ActivityManager().OnActivityJoinRequest.Connect(
        [event_proc](const discord::User &user) {
            if (event_proc == Qnil)
                return;
            
            if (!rb_obj_is_kind_of(event_proc, rb_cProc))
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(event_proc));

            VALUE argv = rb_ary_new();
            Object rb_user = rb_class_new_instance(0, &argv, rb_cUser);
            setPrivateData(rb_user, (void*) &user);

            VALUE array = rb_ary_new_from_args(2, event_proc, rb_user);
            int state;
            rb_protect(rb_discord_call_proc, array, &state);

            LOG_ERROR_IF_STATE;
        }
    );
    event_proc = rb_common_get_event_proc(1, Symbol("on_activity_join_request" + token));

    return token;
}

Object rb_activity_manager_on_activity_join_request_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->ActivityManager().OnActivityJoinRequest.Disconnect(token);
    rb_hash_delete(rb_oProcEvents, Symbol("on_activity_join_request" + token));

    return Qnil;
}

int rb_activity_manager_on_activity_invite_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc;
    int token = DiscordSDK.core->ActivityManager().OnActivityInvite.Connect(
        [event_proc](discord::ActivityActionType action_type, const discord::User &user, const discord::Activity &activity) {
            if (event_proc == Qnil)
                return;

            if (!rb_obj_is_kind_of(event_proc, rb_cProc))
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(event_proc));

            VALUE argv = rb_ary_new();
            Object rb_user = rb_class_new_instance(0, &argv, rb_cUser);
            setPrivateData(rb_user, (void*) &user);

            VALUE argv2 = rb_ary_new();
            Object rb_activity = rb_class_new_instance(0, &argv2, rb_cActivity);
            setPrivateData(rb_activity, (void*) &activity);

            VALUE array = rb_ary_new_from_args(4, event_proc, rb_user, rb_activity, INT2NUM((int) action_type));
            int state;
            rb_protect(rb_discord_call_proc, array, &state);

            LOG_ERROR_IF_STATE;
        }
    );
    event_proc = rb_common_get_event_proc(1, Symbol("on_activity_invite" + token));


    return token;
}

Object rb_activity_manager_on_activity_invite_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->ActivityManager().OnActivityInvite.Disconnect(token);
    rb_hash_delete(rb_oProcEvents, Symbol("on_activity_invite" + token));

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
    rb_mActivityManager.define_module_function(
            "update_activity",
            &rb_activity_manager_update_activity
    );
    rb_mActivityManager.define_module_function(
            "clear_activity",
            &rb_activity_manager_clear_activity
    );
    rb_mActivityManager.define_module_function(
            "send_request_reply",
            &rb_activity_manager_send_request_reply
    );
    rb_mActivityManager.define_module_function(
            "send_invite",
            &rb_activity_manager_send_invite
    );
    rb_mActivityManager.define_module_function(
            "accept_invite",
            &rb_activity_manager_accept_invite
    );
    DEF_METHOD_EVENT(rb_mActivityManager, activity_join, activity_manager);
    DEF_METHOD_EVENT(rb_mActivityManager, activity_spectate, activity_manager);
    DEF_METHOD_EVENT(rb_mActivityManager, activity_join_request, activity_manager);
    DEF_METHOD_EVENT(rb_mActivityManager, activity_invite, activity_manager);
}