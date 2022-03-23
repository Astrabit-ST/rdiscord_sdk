#pragma once

#include <rice/rice.hpp>
#include "rdiscord_sdk.h"

using namespace Rice;

class RubyActivity final {
    void SetType(VALUE type);
    VALUE GetType() const;
};

extern Class rb_cActivity;
void rb_activity_init_class(Module module);