#pragma once

#include <rice/rice.hpp>
#include "rdiscord_sdk.h"
#include "discord.h"
#include "common.h"

using namespace Rice;

extern Class rb_cInputMode;
void rb_input_mode_init_class(Module module);

#define GET_INPUT_MODE(obj) \
if (!obj.is_a(rb_cInputMode)) { \
    rb_raise(rb_eTypeError, "InputMode is not a Discord::InputMode"); \
} \
discord::InputMode* input_mode = getPrivateData<discord::InputMode>(obj);
