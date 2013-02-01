#include "frostbitten.h"
#include "message.h"
VALUE c_frostbitten_message;

void frostbitten_message_deallocate(fb_message *message) {
	message->header = NULL;
	message->packetSize = 0;
	message->wordCount = 0;
	memset(message->words[0], 0, sizeof(&message->words));
	ruby_xfree(message);
}

VALUE frostbitten_message_allocate (VALUE klass) {
	fb_message *msg = ruby_xmalloc(sizeof(fb_message));
	return Data_Wrap_Struct(klass, NULL, frostbitten_message_deallocate, msg);
}

void message_init() {
	c_frostbitten_message = rb_define_class_under(m_frostbitten, "Message", rb_cObject);
	rb_define_alloc_func(c_frostbitten_message, frostbitten_message_allocate);
}