#include "common.h"
#include <functional>
#include <iostream>

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
    if (!proc.is_a(rb_cProc)) {
        rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", proc.class_name());
    }

    proc.call("call", "hi the fuck");
    
    std::function<void(discord::Result)> fn = [&](discord::Result result){
        if ((VALUE) proc == Qnil)
            return;

        VALUE exception = rb_sprintf("[DiscordGameSDK] PROC ", proc.class_of());
        fwrite(StringValuePtr(exception), 1, RSTRING_LEN(exception), stderr);
        rb_set_errinfo(Qnil);

        if (!proc.is_a(rb_cProc)) {
            rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", proc.class_name());
        }

        rb_ary_delete(rb_oProcArray, proc);
        proc.call("call", (int) result);
    };
    return fn;
}

