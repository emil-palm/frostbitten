#include "frostbitten.h"
#include "header.h"
#include <ruby/io.h>
VALUE c_frostbitten_header;


VALUE frostbitten_header_parse_from_buffer(VALUE self, fb_packet_buffer *buffer) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
    frostbitten_header_unpack(header, buffer->header);
    return self;
}

VALUE frostbitten_header_get_sequence(VALUE self) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	return INT2NUM(header->sequence);
}

VALUE frostbitten_header_set_sequence(VALUE self, VALUE seq) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	header->sequence = NUM2INT(seq);
	return frostbitten_header_get_sequence(self);
}

VALUE frostbitten_header_set_origin(VALUE self, VALUE origin) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	if ( ID2SYM(rb_intern("client")) == origin ) {
		header->origin = 1;
	} else if ID2SYM(rb_intern("server") == origin ) {
		header->origin = 0;
	} else {
		rb_raise(rb_eTypeError, "must be either :client or :server");
	}
	return Qfalse;
}

VALUE frostbitten_header_get_origin(VALUE self) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	switch(header->origin) {
		case 1:
			return ID2SYM(rb_intern("client"));
			break;

		case 0:
			return ID2SYM(rb_intern("server"));
			break;
	}
	return Qnil;
}

VALUE frostbitten_header_get_response(VALUE self) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	if ( header->response == 0 ) {
		return Qtrue;
	} else {
		return Qfalse;
	}
}

VALUE frostbitten_header_set_response(VALUE self, VALUE response) {
	if ( TYPE(response) != T_TRUE && TYPE(response) != T_FALSE) {
		rb_raise(rb_eTypeError, "must be either true or false");
	}

	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	if ( response == Qtrue ) 
		header->response = 1;
	else {
		header->response = 0;
	}
	return frostbitten_header_get_response(self);
}

uint32_t frostbitten_header_package(fb_header *header) {
	uint32_t data = header->sequence;
	if ( header->origin == 0 ) {
		data |= 1 << 31;
	} else { 
		data &= ~(1 << 31);
	}

	if ( header->response != (( 1 << 30 ) & data) ) {
		data ^= 1 << 30;
	}

	return data;
}

void frostbitten_header_unpack(fb_header *header, uint32_t val) {
	header->sequence = (uint32_t)val & 0x3fffffff;
	header->origin = (val & (1 << 31));
	header->response = (val & (1 << 30));
}

VALUE frostbitten_header_write_to_io(VALUE self, VALUE io) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);

	rb_io_t *fptr;
	GetOpenFile(io, fptr);
	rb_io_check_writable(fptr);

	FILE *fp = rb_io_stdio_file(fptr);

	uint32_t val = frostbitten_header_package(header);
	fwrite((const void*)&val,sizeof(uint32_t),1,fp);
	return self;
}

VALUE frostbitten_header_cls_read_from_io(VALUE self, VALUE io) {
	VALUE header = rb_obj_alloc(c_frostbitten_header);
    rb_obj_call_init(header, 0, 0);
    return frostbitten_header_read_from_io(header, io);
}

VALUE frostbitten_header_read_from_io(VALUE self, VALUE io) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);

	uint32_t val = 0;

	rb_io_t *fptr;
	GetOpenFile(io, fptr);
	rb_io_check_readable(fptr);

	FILE *fp = rb_io_stdio_file(fptr);
	fread((void*)&val, sizeof(uint32_t),1, fp);
    
    frostbitten_header_unpack(header, val);
    return self;
}

void frostbitten_header_deallocate(fb_header *header) {
	header->sequence = 0;
	header->origin = 0;
	header->response = false;
	ruby_xfree(header);
}

VALUE frostbitten_header_allocate (VALUE klass) {
	fb_header *header = ruby_xmalloc(sizeof(fb_header));
	return Data_Wrap_Struct(klass,NULL,frostbitten_header_deallocate, header);
}

VALUE frostbitten_header_init(int argc, VALUE *argv, VALUE self) {

	fb_header *header;
	Data_Get_Struct(self, fb_header, header);

 	VALUE options;
    if (rb_scan_args(argc, argv, "01", &options) == 0)
        options = Qnil;

    VALUE sequence = INT2NUM(0);
    VALUE origin = ID2SYM(rb_intern("client"));
    VALUE response = Qfalse;

	if (!NIL_P(options) && TYPE(options) == T_HASH) {
		sequence = OVERRIDE_IF_SET(options,sequence);
		origin = OVERRIDE_IF_SET(options,origin);	
		response = OVERRIDE_IF_SET(options,response);		
	}
	frostbitten_header_set_sequence(self,sequence);
	frostbitten_header_set_origin(self,origin);
	frostbitten_header_set_response(self,response);
		
	return self;
}

void header_init() {
	c_frostbitten_header = rb_define_class_under(m_frostbitten, "Header", rb_cObject);
	rb_define_alloc_func(c_frostbitten_header, frostbitten_header_allocate);
	rb_define_method(c_frostbitten_header, "initialize", frostbitten_header_init, -1);

    rb_define_method(c_frostbitten_header,"sequence=", frostbitten_header_set_sequence,1);
    rb_define_method(c_frostbitten_header,"sequence", frostbitten_header_get_sequence,0);
    
    rb_define_method(c_frostbitten_header, "origin", frostbitten_header_get_origin,0);
    rb_define_method(c_frostbitten_header, "origin=", frostbitten_header_set_origin, 1);

	rb_define_method(c_frostbitten_header,"response=", frostbitten_header_set_response, 1);
    rb_define_method(c_frostbitten_header,"is_response?", frostbitten_header_get_response, 0);

    rb_define_method(c_frostbitten_header, "read", frostbitten_header_read_from_io, 1);
    rb_define_method(c_frostbitten_header, "write", frostbitten_header_write_to_io, 1);
    rb_define_singleton_method(c_frostbitten_header, "read", frostbitten_header_cls_read_from_io, 1);

}
