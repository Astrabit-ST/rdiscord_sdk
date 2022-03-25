#pragma once

#include "rdiscord_sdk.h"
#include <exception>

#define CHECK_CORE_INITIALIZED \
    if (!DiscordSDK.core) \
        rb_raise(rb_eRuntimeError, "The Discord SDK core is not initialized!");

template <class C> inline C *getPrivateData(VALUE self) {
    C *c = static_cast<C *>(RTYPEDDATA_DATA(self));
    if (!c) {
        rb_raise(rb_eStandardError, "DATA_PTR for object missing data");
    }
    return c;
}

static inline void setPrivateData(VALUE self, void *p) {
    RTYPEDDATA_DATA(self) = p;
}


#define GUARD_EXC(exp) \
{ \
    try { \
        exp \
    } catch (std::exception &error) { \
        rb_raise(rb_eFatal, "%s", error.what();) \
    } \
}

using namespace Rice;

#define DEF_METHOD_SET(klass, name, subclass) \
klass.define_method(#name, &rb_##subclass##_get_##name);

#define DEF_METHOD_GET(klass, name, subclass) \
klass.define_method(#name "=", &rb_##subclass##_set_##name);

#define DEF_METHOD_GET_SET(klass, name, subclass) \
klass.define_method(#name, &rb_##subclass##_get_##name); \
klass.define_method(#name "=", &rb_##subclass##_set_##name);

Object rb_common_get_proc(int args);
VALUE rb_result_to_obj(discord::Result);
extern VALUE rb_oProcArray;

std::function<void(discord::Result)> rb_discord_callback_wrapper_basic(Object proc);