#include "word.h"
VALUE c_frostbitten_word;
void frostbitten_word_deallocate(fb_word *word) {
	word->len = 0;
	memset(&word->str[0],0,sizeof(&word->str));
	word->term = 0x0;
	ruby_xfree(word);
}

VALUE frostbitten_word_read_from_io(VALUE self, VALUE io) {
	fb_word *word;
	Data_Get_Struct(self, fb_word, word);

	rb_io_t *fptr;
	GetOpenFile(io, fptr);
	rb_io_check_readable(fptr);

	FILE *fp = rb_io_stdio_file(fptr);
	fread((void*)&word->len, sizeof(uint32_t),1, fp);
	fread((void*)&word->str, sizeof(char),word->len, fp);
	fread((void*)&word->term, sizeof(char), 1, fp);
	printf("%s", word->str);
    return self;
}

VALUE frostbitten_word_cls_read_from_io(VALUE self, VALUE io) {
	return self;
}

VALUE frostbitten_word_write_to_io(VALUE self, VALUE io) {
	fb_word *word;
	Data_Get_Struct(self, fb_word, word);

	rb_io_t *fptr;
	GetOpenFile(io, fptr);
	rb_io_check_writable(fptr);

	FILE *fp = rb_io_stdio_file(fptr);
	// fwrite((const void*)&val,sizeof(uint32_t),1,fp);

	rb_io_bufwrite(io, (const void*)&word->len, sizeof(uint32_t));
	rb_io_bufwrite(io, (const void*)&word->str, sizeof(uint32_t));
	rb_io_bufwrite(io, (const void*)&word->len, sizeof(uint32_t));
	return self;
}

VALUE frostbitten_word_allocate (VALUE klass) {
	fb_word *word = ruby_xmalloc(sizeof(fb_word));
	return Data_Wrap_Struct(klass, NULL, frostbitten_word_deallocate, word);
}

void word_init() {
	c_frostbitten_word = rb_define_class_under(m_frostbitten, "Word", rb_cString);
	rb_define_alloc_func(c_frostbitten_word, frostbitten_word_allocate);

	rb_define_method(c_frostbitten_word, "read", frostbitten_word_read_from_io, 1);
    rb_define_method(c_frostbitten_word, "write", frostbitten_word_write_to_io, 1);
    rb_define_singleton_method(c_frostbitten_word, "read", frostbitten_word_cls_read_from_io, 1);

}