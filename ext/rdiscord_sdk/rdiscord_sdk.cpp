#include <rice/rice.hpp>
#include "rdiscord_sdk.h"

extern "C"
using namespace Rice;

Object initialize(Object /*self */, long client_id, long flags) {
    if (rb_oDiscordCore) {
        rb_raise(rb_eRuntimeError, "Discord already initialized!");
    }
    discord::Core* rb_oDiscordCore;
    auto result = discord::Core::Create(client_id, flags, &rb_oDiscordCore);
    return INT2NUM((int) result);
}

void Init_rdiscord_sdk()
{
    Module rb_mDiscord = define_module("Discord");
    rb_mDiscord.define_module_function("init", &initialize);
}