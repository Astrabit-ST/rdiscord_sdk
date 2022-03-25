#pragma once

#include <rice/rice.hpp>
#include "rdiscord_sdk.h"

using namespace Rice;

#define GET_ACTIVITY(obj) \
 discord::Activity* activity = getPrivateData<discord::Activity>(obj);

extern Class rb_cActivity;
void rb_activity_init_class(Module module);