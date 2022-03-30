#include "input_mode.h"
#include <rice/rice.hpp>
#include "discord.h"
#include "common.h"
#include <cstdint>
using namespace Rice;

Class rb_cInputMode;

DEF_TYPE_CUSTOMTYPENAME(discord::InputMode, InputMode_rbType);

Object rb_input_mode_initialize(Object self) {
    discord::InputMode* input_mode = new discord::InputMode();
    setPrivateData(self, input_mode);

    return self;
}

int rb_input_mode_get_type(Object self) {
    GET_INPUT_MODE(self);

    return (int) input_mode->GetType();
}

Object rb_input_mode_set_type(Object self, int type) {
    GET_INPUT_MODE(self);

    input_mode->SetType((discord::InputModeType) type);

    return Qnil;
}

const char* rb_input_mode_get_shortcut(Object self) {
    GET_INPUT_MODE(self);

    return input_mode->GetShortcut();
}

Object rb_input_mode_set_shortcut(Object self, const char* shortcut) {
    GET_INPUT_MODE(self);

    input_mode->SetShortcut(shortcut);

    return Qnil;
}

void rb_input_mode_init_class(Module module) {
    rb_cInputMode = define_class_under(module, "InputMode");
    rb_define_alloc_func(rb_cInputMode, classAllocate<&InputMode_rbType>);
    rb_cInputMode.define_method("initialize", &rb_input_mode_initialize);
    DEF_METHOD_GET_SET(rb_cInputMode, type, input_mode);
    DEF_METHOD_GET_SET(rb_cInputMode, shortcut, input_mode);
}