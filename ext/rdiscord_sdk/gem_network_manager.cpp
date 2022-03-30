#include "gem_network_manager.h"
#include <rice/rice.hpp>
#include "common.h"
#include "discord.h"
#include <cstdint>
#include <iostream>

Module rb_mNetworkManager;

std::uint64_t rb_network_manager_get_peer_id() {
    CHECK_CORE_INITIALIZED;

    std::uint64_t peerId;

    DiscordSDK.core->NetworkManager().GetPeerId(&peerId);

    return peerId;
}

int rb_network_manager_flush() {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->NetworkManager().Flush();
}

int rb_network_manager_open_peer(std::uint64_t peer_id, const char* route_data) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->NetworkManager().OpenPeer(peer_id, route_data);
}

int rb_network_manager_update_peer(std::uint64_t peer_id, const char* route_data) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->NetworkManager().UpdatePeer(peer_id, route_data);
}

int rb_network_manager_close_peer(std::uint64_t peer_id) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->NetworkManager().ClosePeer(peer_id);
}

int rb_network_manager_open_channel(std::uint64_t peer_id, std::uint8_t channel_id, bool reliable) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->NetworkManager().OpenChannel(peer_id, channel_id, reliable);
}

int rb_network_manager_close_channel(std::uint64_t peer_id, std::uint8_t channel_id) {
    CHECK_CORE_INITIALIZED;

    return (int) DiscordSDK.core->NetworkManager().CloseChannel(peer_id, channel_id);
}

int rb_network_manager_send_message(std::uint64_t peer_id, std::uint8_t channel_id, Array rb_data) {
    CHECK_CORE_INITIALIZED;

    std::uint32_t data_length = rb_array_len(rb_data);
    std::uint8_t data[data_length];

    int value;
    for (std::uint32_t i = 0; i < data_length; i++) {
        value = NUM2INT(
            rb_ary_entry(rb_data, i)
        );
        if (value > 255 || value < 0) {
            rb_raise(rb_eRangeError, "Expected Integer between 0 and 255, encountered %d at index %d", value, i);
            return 4;
        }
        data[i] = (std::uint8_t) value;
    }

    return (int) DiscordSDK.core->NetworkManager().SendMessage(peer_id, channel_id, data, data_length);
}


int rb_network_manager_on_message_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc  = rb_common_get_event_proc(4);
    int token = DiscordSDK.core->NetworkManager().OnMessage.Connect(
        [event_proc](std::uint64_t peer_id, std::uint8_t channel_id, std::uint8_t* data, std::uint32_t data_length) {
            if (event_proc == Qnil)
                return;

            if (!rb_obj_is_kind_of(event_proc, rb_cProc))
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(event_proc));

            VALUE rb_data = rb_ary_new();
            for (int i = 0; i < data_length; i++) {
                rb_ary_push(rb_data, INT2NUM(data[i]));
            }

            VALUE array = rb_ary_new_from_args(4, event_proc, INT2NUM(peer_id), INT2NUM(channel_id), rb_data);

            int state;
            state = rb_discord_call_proc(array);

            LOG_ERROR_IF_STATE;
        }
    );
    rb_common_add_proc_to_hash(Symbol("on_message" + token), event_proc);

    return token;
}

Object rb_network_manager_on_message_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->NetworkManager().OnMessage.Disconnect(token);
    rb_hash_delete(rb_oProcEvents, Symbol("on_message" + token));

    return Qnil;
}

int rb_network_manager_on_route_update_connect() {
    CHECK_CORE_INITIALIZED;

    VALUE event_proc  = rb_common_get_event_proc(1);
    int token = DiscordSDK.core->NetworkManager().OnRouteUpdate.Connect(
        [event_proc](const char* route_data) {
            if (event_proc == Qnil)
                return;

            if (!rb_obj_is_kind_of(event_proc, rb_cProc))
                rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(event_proc));

            VALUE array = rb_ary_new_from_args(2, event_proc, rb_str_new_cstr(route_data));

            int state;
            state = rb_discord_call_proc(array);

            LOG_ERROR_IF_STATE;
        }
    );
    rb_common_add_proc_to_hash(Symbol("on_route_update" + token), event_proc);

    return token;
}

Object rb_network_manager_on_route_update_disconnect(int token) {
    CHECK_CORE_INITIALIZED;

    DiscordSDK.core->NetworkManager().OnRouteUpdate.Disconnect(token);
    rb_hash_delete(rb_oProcEvents, Symbol("on_route_update" + token));

    return Qnil;
}

void rb_network_init_manager(Module module) {
    rb_mNetworkManager = define_module_under(module, "NetworkManager");
    rb_mNetworkManager.define_module_function("get_peer_id", &rb_network_manager_get_peer_id);
    rb_mNetworkManager.define_module_function("flush", &rb_network_manager_flush);
    rb_mNetworkManager.define_module_function("open_peer", &rb_network_manager_open_peer);
    rb_mNetworkManager.define_module_function("update_peer", &rb_network_manager_update_peer);
    rb_mNetworkManager.define_module_function("close_peer", &rb_network_manager_close_peer);
    rb_mNetworkManager.define_module_function("open_channel", &rb_network_manager_open_channel);
    rb_mNetworkManager.define_module_function("close_channel", &rb_network_manager_close_channel);
    rb_mNetworkManager.define_module_function("send_message", &rb_network_manager_send_message);
    DEF_METHOD_EVENT(rb_mNetworkManager, message, network_manager);
    DEF_METHOD_EVENT(rb_mNetworkManager, route_update, network_manager);
}