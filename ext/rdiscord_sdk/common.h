#pragma once

#include "rdiscord_sdk.h"
#include <exception>

#define BOOL2RB(val) ((val) ? Qtrue : Qfalse)

#define CHECK_CORE_INITIALIZED \
    if (!DiscordSDK.core) \
        rb_raise(rb_eRuntimeError, "The Discord SDK core is not initialized!");

template <class C> inline C *getPrivateData(VALUE self) {
    C *c = static_cast<C *>(RTYPEDDATA_DATA(self));
    if (!c) {
        rb_raise(rb_eStandardError, "RTYPEDDATA_DATA for object missing data");
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

template <rb_data_type_t *rbType> static VALUE classAllocate(VALUE klass) {
    return rb_data_typed_object_wrap(klass, 0, rbType);
}

template <class C> static void freeInstance(void *inst) {
    delete static_cast<C *>(inst);
}

#define DEF_TYPE_FLAGS 0

#define DEF_TYPE_CUSTOMNAME_AND_FREE(Klass, Name, Free, Typename) \
rb_data_type_t Typename = {Name, {0, Free, 0, 0, 0}, 0, 0, DEF_TYPE_FLAGS}

#define DEF_TYPE_CUSTOMTYPENAME(Klass, Typename) \
DEF_TYPE_CUSTOMNAME_AND_FREE(Klass, #Klass, freeInstance<Klass>, Typename)

#define DEF_TYPE_CUSTOMNAME(Klass, Name) \
DEF_TYPE_CUSTOMNAME_AND_FREE(Klass, Name, freeInstance<Klass>, Klass##_rbType)

#define DEF_TYPE(Klass) DEF_TYPE_CUSTOMNAME(Klass, #Klass)

using namespace Rice;

#define DEF_METHOD_SET(klass, name, subclass) \
klass.define_method(#name, &rb_##subclass##_get_##name);

#define DEF_METHOD_GET(klass, name, subclass) \
klass.define_method(#name "=", &rb_##subclass##_set_##name);

#define DEF_METHOD_GET_SET(klass, name, subclass) \
klass.define_method(#name, &rb_##subclass##_get_##name); \
klass.define_method(#name "=", &rb_##subclass##_set_##name);

#define DEF_METHOD_EVENT(klass, name, subclass) \
klass.define_module_function("on_" #name "_connect", &rb_##subclass##_on_##name##_connect); \
klass.define_module_function("on_" #name "_disconect", &rb_##subclass##_on_##name##_disconnect);

#define LOG_ERROR_IF_STATE \
if (state) { \
    /* callback function broke, theres not much we can do other than print out an error */  \
    VALUE exception = rb_sprintf("[DiscordGameSDK] Callback function error: %" PRIsVALUE, rb_errinfo()); \
    fwrite(StringValuePtr(exception), 1, RSTRING_LEN(exception), stderr); \
    rb_set_errinfo(Qnil); \
}

VALUE rb_common_get_callback_proc(int args);
VALUE rb_common_get_event_proc(int args);
void rb_common_add_proc_to_hash(VALUE key, VALUE proc);

VALUE rb_result_to_obj(discord::Result);
extern VALUE rb_oPendingCallbacks;
extern VALUE rb_oProcEvents;

VALUE rb_discord_call_proc(VALUE ary);
std::function<void(discord::Result)> rb_discord_callback_wrapper_basic(VALUE proc);