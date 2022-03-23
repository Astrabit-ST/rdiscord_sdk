#include "common.h"

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