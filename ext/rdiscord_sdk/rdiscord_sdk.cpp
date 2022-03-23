#include <rice/rice.hpp>

extern "C"
void Init_rdiscord_sdk()
{
    Rice::Module rb_mDiscord = Rice::define_module("Discord");
}