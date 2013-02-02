#include "frostbitten.h"
#include "message.h"
VALUE c_frostbitten_message;

char** frostbitten_message_generate(fb_message *message, uint32_t *message_size);

VALUE frostbitten_message_read_from_io(VALUE self, VALUE io) {
	fb_message *message;
	Data_Get_Struct(self, fb_message, message);
	message->header = rb_class_new_instance(0, NULL, c_frostbitten_header);
	frostbitten_header_read_from_io(message->header, io);
	

	rb_io_t *fptr;
	GetOpenFile(io, fptr);
	rb_io_check_readable(fptr);
	uint32_t packetSize, wordCount = 0;

	FILE *fp = rb_io_stdio_file(fptr);

	fpos_t pos;
	

	fread((void*)&packetSize, sizeof(uint32_t),1, fp);
	fread((void*)&wordCount, sizeof(uint32_t),1, fp);



	uint32_t bufferSize = packetSize-12; // Remove the byte size of header, packetSize and wordCount
	
	for(uint32_t i=0; i < wordCount; i++) {
		uint32_t wordByteLength = 0;
		fread(&wordByteLength, sizeof(uint32_t), 1, fp);

		char *word = malloc(wordByteLength+1);
		fread(word, sizeof(char), wordByteLength+1, fp);
		rb_ary_push(message->words, rb_str_new2(word));
	}
    
    return self;
}

VALUE frostbitten_message_write_to_io(VALUE self, VALUE io) {
	fb_message *message;
	Data_Get_Struct(self, fb_message, message);

	frostbitten_header_write_to_io(message->header, io);


	uint32_t arrayLength = (uint32_t)RARRAY_LEN(message->words);
	uint32_t packetSize = 0;

	char** strings = frostbitten_message_generate(message, &packetSize);
	packetSize += sizeof(uint32_t); // Header
	packetSize += sizeof(uint32_t); // packetSize
	packetSize += sizeof(uint32_t); // wordCount
	rb_io_t *fptr;
	GetOpenFile(io, fptr);
	rb_io_check_readable(fptr);
	FILE *fp = rb_io_stdio_file(fptr);

	fwrite((const void*)&packetSize,sizeof(uint32_t),1,fp);
	fwrite((const void*)&arrayLength,sizeof(uint32_t),1,fp);
	for(uint32_t i=0; i < arrayLength; i++) {
		char *str = strings[i];
		uint32_t len = (uint32_t)strlen(str);
		fwrite((const void*)&len, sizeof(uint32_t), 1, fp);
		fwrite(str, sizeof(char), strlen(str)+1, fp);
	}
	fflush(fp);
	return self;
}

char** frostbitten_message_generate(fb_message *message, uint32_t *message_size) {
	uint32_t arrayLength = (uint32_t)RARRAY_LEN(message->words);
	char **strings = (char **)calloc(arrayLength,sizeof(char *));

	for(uint32_t i=0; i < arrayLength; i++) {
		VALUE str = rb_ary_entry(message->words, i);
		if ( !NIL_P(str) && TYPE(str) == T_STRING ) {
			char *p = StringValueCStr(str);
			char *p2 = malloc(strlen(p)*sizeof(char)+1);
			memcpy(p2,p,strlen(p)*sizeof(char)+1);
			strings[i] = p2;
			*message_size += sizeof(char)*(strlen(p2)+1)+sizeof(uint32_t);
		}
	}
	return strings;
}

VALUE frostbitten_message_get_words(VALUE self) {
	fb_message *message;
	Data_Get_Struct(self, fb_message, message);
	return message->words;
}

VALUE frostbitten_message_set_words(VALUE self, VALUE words) {
	Check_Type(words, T_ARRAY);

	fb_message *message;
	Data_Get_Struct(self, fb_message, message);
	rb_gc_mark(message->words);
	message->words = words;
	return message->words;
}

VALUE frostbitten_message_get_header(VALUE self) {
	fb_message *message;
	Data_Get_Struct(self, fb_message, message);
	return message->header;
}

VALUE frostbitten_message_set_header(VALUE self, VALUE header) {
	if ( NIL_P(header) || CLASS_OF(header) != c_frostbitten_header ) {
		rb_raise(rb_eTypeError, "Invalid argument header, not a Frostbitten::Header");
		return Qnil;
	}
	fb_message *message;
	Data_Get_Struct(self, fb_message, message);
	message->header = header;
	return message->header;
}

VALUE frostbitten_message_init(VALUE self) {

	fb_message *message;
	Data_Get_Struct(self, fb_message, message);
	message->words = rb_ary_new();
	message->header = rb_obj_alloc(c_frostbitten_header);
	return self;
}

void frostbitten_message_deallocate(fb_message *message) {
	// frostbitten_header_deallocate(message->header);
	ruby_xfree(message);
}

void frostbitten_message_mark(fb_message *message) {
	rb_gc_mark(message->header);
	rb_gc_mark(message->words);
}


VALUE frostbitten_message_allocate (VALUE klass) {
	fb_message *msg = ruby_xmalloc(sizeof(fb_message));
	return Data_Wrap_Struct(klass, frostbitten_message_mark, frostbitten_message_deallocate, msg);
}

void message_init() {
	c_frostbitten_message = rb_define_class_under(m_frostbitten, "Message", rb_cObject);
	rb_define_alloc_func(c_frostbitten_message, frostbitten_message_allocate);
	rb_define_method(c_frostbitten_message, "initialize", frostbitten_message_init, 0);

	rb_define_method(c_frostbitten_message, "write", frostbitten_message_write_to_io, 1);
	rb_define_method(c_frostbitten_message, "read", frostbitten_message_read_from_io, 1);
	
	rb_define_method(c_frostbitten_message, "words", frostbitten_message_get_words, 0);
	rb_define_method(c_frostbitten_message, "words=", frostbitten_message_set_words, 1);

	rb_define_method(c_frostbitten_message, "header", frostbitten_message_get_header, 0);
	rb_define_method(c_frostbitten_message, "header=", frostbitten_message_set_header, 1);

}
