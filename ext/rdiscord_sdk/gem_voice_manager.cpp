#include "gem_voice_manager.h"
#include <rice/rice.hpp>
#include "common.h"
#include "discord.h"
#include "input_mode.h"
#include <cstdint>

using namespace Rice;

Module rb_mVoiceManager;

Object rb_voice_manager_get_input_mode() {
    CHECK_CORE_INITIALIZED;

    discord::InputMode* input_mode = new discord::InputMode();
    discord::Result result = DiscordSDK.core->VoiceManager().GetInputMode(input_mode);

    VALUE argv = rb_ary_new();
    VALUE rb_input_mode = rb_class_new_instance(0, &argv, rb_cInputMode);
    setPrivateData(rb_input_mode, (void*) input_mode);

    return rb_ary_new_from_args(2, (int) result, rb_input_mode);
}

Object rb_voice_manager_set_input_mode(Object rb_input_mode) {
    CHECK_CORE_INITIALIZED;
    GET_INPUT_MODE(rb_input_mode);

    VALUE callback_proc = rb_common_get_callback_proc(1);

    DiscordSDK.core->VoiceManager().SetInputMode(*input_mode, rb_discord_callback_wrapper_basic(callback_proc));

    return Qnil;
}

Object rb_voice_manager_get_self_mute() {
    CHECK_CORE_INITIALIZED;

    bool self_mute;
    discord::Result result = DiscordSDK.core->VoiceManager().IsSelfMute(&self_mute);

    return rb_ary_new_from_args(2, (int) result, self_mute);
}

int rb_voice_manager_set_self_mute(bool self_mute) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->VoiceManager().SetSelfMute(self_mute);    
}

Object rb_voice_manager_get_self_deaf() {
    CHECK_CORE_INITIALIZED;

    bool self_deaf;
    discord::Result result = DiscordSDK.core->VoiceManager().IsSelfDeaf(&self_deaf);

    return rb_ary_new_from_args(2, (int) result, self_deaf);
}

int rb_voice_manager_set_self_deaf(bool self_deaf) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->VoiceManager().SetSelfDeaf(self_deaf);
}

Object rb_voice_manager_get_local_mute(std::int64_t user_id) {
    CHECK_CORE_INITIALIZED;

    bool local_mute;
    discord::Result result = DiscordSDK.core->VoiceManager().IsLocalMute(user_id, &local_mute);

    return rb_ary_new_from_args(2, (int) result, local_mute);
}

int rb_voice_manager_set_local_mute(std::int64_t user_id, bool local_mute) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->VoiceManager().SetLocalMute(user_id, local_mute);
}

Object rb_voice_manager_get_local_volume(std::int64_t user_id) {
    CHECK_CORE_INITIALIZED;

    std::uint8_t local_volume;
    discord::Result result = DiscordSDK.core->VoiceManager().GetLocalVolume(user_id, &local_volume);

    return rb_ary_new_from_args(2, (int) result, local_volume);
}

int rb_voice_manager_set_local_volume(std::int64_t user_id, std::uint8_t local_volume) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->VoiceManager().SetLocalVolume(user_id, local_volume);
}

int rb_voice_manager_on_settings_update_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc  = rb_common_get_event_proc(0);
    int token = DiscordSDK.core->VoiceManager().OnSettingsUpdate.Connect(
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
    rb_common_add_proc_to_hash(Symbol("on_settings_update" + token), event_proc);

    return token;
}

Object rb_voice_manager_on_settings_update_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->VoiceManager().OnSettingsUpdate.Disconnect(token);
    rb_hash_delete(rb_oProcEvents, Symbol("on_settings_update" + token));

    return Qnil;
}

void rb_voice_init_manager(Module module) {
    rb_mVoiceManager = define_module_under(module, "VoiceManager");
    DEF_MODULEFUNC_GET_SET(rb_mVoiceManager, input_mode, voice_manager);
    DEF_MODULEFUNC_GET_SET(rb_mVoiceManager, self_mute, voice_manager);
    DEF_MODULEFUNC_GET_SET(rb_mVoiceManager, self_deaf, voice_manager);
    DEF_MODULEFUNC_GET_SET_BLAND(rb_mVoiceManager, local_mute, voice_manager);
    DEF_MODULEFUNC_GET_SET_BLAND(rb_mVoiceManager, local_volume, voice_manager);
    DEF_METHOD_EVENT(rb_mVoiceManager, settings_update, voice_manager);
}