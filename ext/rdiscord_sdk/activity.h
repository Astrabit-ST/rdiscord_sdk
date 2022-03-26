#pragma once

#include <rice/rice.hpp>
#include "rdiscord_sdk.h"
#include "discord.h"
#include "common.h"

using namespace Rice;

extern Class rb_cActivity;
void rb_activity_init_class(Module module);

#define GET_ACTIVITY(obj) \
if (!obj.is_a(rb_cActivity)) { \
    rb_raise(rb_eTypeError, "Activity is not a Discord::Activity"); \
} \
discord::Activity* activity = getPrivateData<discord::Activity>(obj);
