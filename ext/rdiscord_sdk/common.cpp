#include "common.h"
#include <functional>
#include <iostream>

using namespace Rice;

VALUE rb_common_get_callback_proc(int args) {
    if (!rb_block_given_p())
    {
        return Qnil;
    }
    VALUE proc = rb_block_proc();
    if (rb_proc_arity(proc) != args) {
        rb_raise(rb_eRuntimeError, "Passed block should take %d arguments, got %d instead", args, rb_proc_arity(proc));
    }
    rb_ary_push(rb_oPendingCallbacks, proc);
    return proc;
}

VALUE rb_common_get_event_proc(int args) {
    if (!rb_block_given_p())
    {
        return Qnil;
    }

    VALUE proc = rb_block_proc();
    if (rb_proc_arity(proc) != args) {
        rb_raise(rb_eRuntimeError, "Passed block should take %d arguments, got %d instead", args, rb_proc_arity(proc));
    }
    return proc;
}

void rb_common_add_proc_to_hash(VALUE key, VALUE proc) {
    rb_hash_aset(rb_oProcEvents, key, proc);
}

VALUE rb_result_to_obj(discord::Result result) {
    return INT2NUM((int) result);
}

VALUE rb_discord_call_proc(VALUE ary) {
    VALUE proc = rb_ary_shift(ary);
    return rb_proc_call(proc, ary);
}

/*  Discord's C++ API doesn't have callback data, so this is the easiest way (i could think of) to make a callback know what proc it has */
std::function<void(discord::Result)> rb_discord_callback_wrapper_basic(VALUE proc) {    
    std::function<void(discord::Result)> fn = [proc](discord::Result result){
        if (proc == Qnil)
            return;

        if (!rb_obj_is_kind_of(proc, rb_cProc)) {
            rb_raise(rb_eTypeError, "Proc expected in callback, got %d instead", rb_obj_classname(proc));
        }

        rb_ary_delete(rb_oPendingCallbacks, proc);
        VALUE ary = rb_ary_new_from_args(2, proc, INT2NUM((int) result));

        int state;
        rb_protect(rb_discord_call_proc, ary, &state);
        LOG_ERROR_IF_STATE;
    };
    return fn;
}

