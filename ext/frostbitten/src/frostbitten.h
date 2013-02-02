#ifndef frostbitten_h
#define frostbitten_h
#include <ruby.h>
#include <ruby/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h> 
extern VALUE m_frostbitten;
void Init_frostbitten();
#define OVERRIDE_IF_SET(options,name)   rb_funcall(options, rb_intern("has_key?"), 1, ID2SYM(rb_intern(#name))) == Qtrue ? \
                                rb_hash_aref(options, ID2SYM(rb_intern(#name))) : name
#endif

