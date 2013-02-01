#include "word.h"
VALUE c_frostbitten_word;
void frostbitten_word_deallocate(fb_word *word) {
	word->len = 0;
	memset(&word->str[0],0,sizeof(&word->str));
	word->term = 0x0;
	ruby_xfree(word);
}

VALUE frostbitten_word_allocate (VALUE klass) {
	fb_word *word = ruby_xmalloc(sizeof(fb_word));
	return Data_Wrap_Struct(klass, NULL, frostbitten_word_deallocate, word);
}

void word_init() {
	c_frostbitten_word = rb_define_class_under(m_frostbitten, "Word", rb_cObject);
	rb_define_alloc_func(c_frostbitten_word, frostbitten_word_allocate);
}