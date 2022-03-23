#include <rice/rice.hpp>
#include <discord.h>

using namespace Rice;

Module rb_mDiscord;
discord::Core* rb_oDiscordCore;

void Init_rdiscord_sdk();