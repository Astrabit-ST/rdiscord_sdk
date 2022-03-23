#pragma once

#include "rdiscord_sdk.h"

#define CHECK_CORE_INITIALIZED \
    if (!DiscordSDK.core) \
        rb_raise(rb_eRuntimeError, "The Discord SDK core is not initialized!");

using namespace Rice;

Object rb_common_get_proc(int args);
extern VALUE rb_oProcArray;