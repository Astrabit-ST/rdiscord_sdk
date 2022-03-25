#include "common.h"
#include <functional>

using namespace Rice;

Object rb_common_get_proc(int args) {
    if (!rb_block_given_p())
    {
        return Qnil;
    }
    Object proc = rb_block_proc();
    if (rb_proc_arity(proc) != args) {
        rb_raise(rb_eRuntimeError, "Passed block should take %d arguments, got %d instead", args, rb_proc_arity(proc));
    }
    rb_ary_push(rb_oProcArray, proc);
    return proc;
}

VALUE rb_result_to_obj(discord::Result result) {
    return INT2NUM((int) result);
}

VALUE rb_discord_call_callback(VALUE ary) {
    VALUE proc = rb_ary_shift(ary);
    return rb_proc_call(proc, ary);
}

/*  Discord's C++ API doesn't have callback data, so this is the easiest way (i could think of) to make a callback know what proc it has */
std::function<void(discord::Result)> rb_discord_callback_wrapper_basic(Object proc) {
    std::function<void(discord::Result)> fn = [&](discord::Result result){
        if ((VALUE) proc == Qnil)
            return;

        rb_ary_delete(rb_oProcArray, proc);
        VALUE ary = rb_ary_new_from_args(2, proc, INT2NUM((int) result));

            int state;
        rb_protect(rb_discord_call_callback, ary, &state);
        if (state) {
            /* callback function broke, theres not much we can do other than print out an error */
            VALUE exception = rb_sprintf("[DiscordGameSDK] Callback function error: %" PRIsVALUE, rb_errinfo());
            fwrite(StringValuePtr(exception), 1, RSTRING_LEN(exception), stderr);
            rb_set_errinfo(Qnil);
        }
    };
    return fn;
}

