#include "gem_user_manager.h"
#include "common.h"
#include "user.h"
#include "discord.h"

using namespace Rice;

Module rb_mUserManager;



// Thanks discord for not making this behave like a callback!!
Object rb_user_manager_get_current_user() {
    CHECK_CORE_INITIALIZED;

    discord::User* user = new discord::User();
    discord::Result result = DiscordSDK.core->UserManager().GetCurrentUser(user);

    VALUE argv = rb_ary_new();
    VALUE rb_user = rb_class_new_instance(0, &argv, rb_cUser);
    setPrivateData(rb_user, (void*) user);

    return rb_ary_new_from_args(2, (int) result, rb_user);
}

Object rb_user_manager_get_user(std::int64_t id) {
    CHECK_CORE_INITIALIZED;

    VALUE callback_proc = rb_common_get_callback_proc(2);
    DiscordSDK.core->UserManager().GetUser(id, 
        [callback_proc](discord::Result result, const discord::User &user){
            if (callback_proc == Qnil)
               return;

            if (!rb_obj_is_kind_of(callback_proc, rb_cProc)) {
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(callback_proc));
            }

            VALUE argv = rb_ary_new();
            VALUE rb_user = rb_class_new_instance(0, &argv, rb_cUser);
            setPrivateData(rb_user, (void*) &user);

            rb_ary_delete(rb_oPendingCallbacks, callback_proc);
            VALUE ary = rb_ary_new_from_args(3, callback_proc, INT2NUM((int) result), rb_user);

            int state;
            rb_protect(rb_discord_call_proc, ary, &state);
            LOG_ERROR_IF_STATE;
        }
    );

    return Qnil;
}

Object rb_user_manager_get_current_user_premium_type() {
    CHECK_CORE_INITIALIZED;

    discord::PremiumType premium_type;
    discord::Result result = DiscordSDK.core->UserManager().GetCurrentUserPremiumType(&premium_type);

    return rb_ary_new_from_args(2, INT2NUM((int) result), INT2NUM((int) premium_type));
}

Object rb_user_manager_current_user_has_flag(int flag) {
    CHECK_CORE_INITIALIZED;

    bool has_flag;
    discord::Result result = DiscordSDK.core->UserManager().CurrentUserHasFlag((discord::UserFlag) flag, &has_flag);

    return rb_ary_new_from_args(2, INT2NUM((int) result), BOOL2RB(has_flag));
}

int rb_user_manager_on_current_user_update_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc  = rb_common_get_event_proc(0);
    int token = DiscordSDK.core->UserManager().OnCurrentUserUpdate.Connect(
        [event_proc]() {
            if (event_proc == Qnil)
                return;
            
            if (!rb_obj_is_kind_of(event_proc, rb_cProc))
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(event_proc));

            
            VALUE array = rb_ary_new_from_args(1, event_proc);
            int state;
            rb_protect(rb_discord_call_proc, array, &state);

            LOG_ERROR_IF_STATE;
        }
    );
    rb_common_add_proc_to_hash(Symbol("on_current_user_update" + token), event_proc);
    
    return token;
}

Object rb_user_manager_on_current_user_update_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->UserManager().OnCurrentUserUpdate.Disconnect(token);
    rb_hash_delete(rb_oPendingCallbacks, Symbol("on_current_user_update" + token));

    return Qnil;
}

void rb_user_init_manager(Module module) {
    rb_mUserManager = define_module_under(module, "UserManager");
    rb_mUserManager.define_module_function("get_current_user", &rb_user_manager_get_current_user);
    rb_mUserManager.define_module_function("get_user", &rb_user_manager_get_user);
    rb_mUserManager.define_module_function("get_current_user_premium_type", &rb_user_manager_get_current_user_premium_type);
    rb_mUserManager.define_module_function("current_user_has_flag", &rb_user_manager_current_user_has_flag);
    DEF_METHOD_EVENT(rb_mUserManager, current_user_update, user_manager);
}