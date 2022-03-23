#pragma once

#include <rice/rice.hpp>
#include <discord.h>

using namespace Rice;

extern Module rb_mDiscord;

struct DiscordStruct {
    std::unique_ptr<discord::Core> core;
    Object log_hook;
};
extern DiscordStruct DiscordSDK;

extern "C"
void Init_rdiscord_sdk();