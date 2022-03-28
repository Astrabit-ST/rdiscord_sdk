#pragma once

#include <rice/rice.hpp>
#include "rdiscord_sdk.h"
#include "discord.h"
#include "common.h"

using namespace Rice;

extern Class rb_cUser;
void rb_user_init_class(Module module);

Object rb_user_initialize(Object self);

#define GET_USER(obj) \
if (!obj.is_a(rb_cUser)) { \
    rb_raise(rb_eTypeError, "User is not a Discord::User"); \
} \
discord::User* user = getPrivateData<discord::User>(obj);
