#include "frostbitten.h"
#include "header.h"
#include <ruby/io.h>
VALUE c_frostbitten_header;

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

VALUE frostbitten_header_get_clientOrigin(VALUE self) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	return INT2NUM(header->clientOrigin);
}

VALUE frostbitten_header_set_clientOrigin(VALUE self, VALUE clientOrigin) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	header->clientOrigin = NUM2INT(clientOrigin);
	return frostbitten_header_get_clientOrigin(self);
}

uint32_t frostbitten_header_package(fb_header *header) {
	uint32_t data = header->sequence;

	if ( header->clientOrigin != (( 1 << 30 ) & data) ) {
		data ^= 1 << 30;
	}

	if ( header->response != (( 1 << 31 ) & data) ) {
		data ^= 1 << 31;
	}

	return data;
}

void frostbitten_header_unpack(fb_header *header, uint32_t val) {
	header->sequence = (uint32_t)val & 0x3fffffff;
	header->clientOrigin = (val & (1 << 30));
	header->response = (val & (1 << 31));
}

VALUE frostbitten_header_write_to_io(VALUE self, VALUE io) {
	fb_header *header;
	Data_Get_Struct(self, fb_header, header);

	uint32_t val = frostbitten_header_package(header);
	rb_io_bufwrite(io, (const void*)&val, sizeof(uint32_t));
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

// VALUE frostbitten_header_test_write(VALUE self) {

// 	fb_header *header;
// 	Data_Get_Struct(self, fb_header, header);

// 	FILE *fp = fopen("path_to_file.bin","wb");
//     if(fp == NULL) {
//     	printf("error creating file");
//     	return -1;
//     }
//    	uint32_t val = frostbitten_header_package(header);
//     fwrite((const void*)&val,sizeof(uint32_t),1,fp);
//     fclose(fp);
//     return Qnil;
// }

// VALUE frostbitten_header_test_read(VALUE self) {
// 	fb_header *header;
// 	Data_Get_Struct(self, fb_header, header);

// 	FILE *fp = fopen("path_to_file.bin","rb");
//     if(fp == NULL) {
//     	printf("error reading file");
//     	return -1;
//     }

//    	uint32_t val = 0;
//    	fread((void*)&val, sizeof(uint32_t),1, fp);
    
//     frostbitten_header_unpack(header, val);
//     fclose(fp);
// 	return self;
// }



VALUE frostbitten_header_format(VALUE self) {

	fb_header *header;
	Data_Get_Struct(self, fb_header, header);
	uint32_t data = header->sequence;

	if ( header->clientOrigin != (( 1 << 30 ) & data) ) {
		data ^= 1 << 30;
	}

	if ( header->response != (( 1 << 31 ) & data) ) {
		data ^= 1 << 31;
	}

	return INT2NUM(data);
}

void frostbitten_header_deallocate(fb_header *header) {
	header->sequence = 0;
	header->clientOrigin = false;
	header->response = false;
	ruby_xfree(header);
}

VALUE frostbitten_header_allocate (VALUE klass) {
	fb_header *header = ruby_xmalloc(sizeof(fb_header));
	return Data_Wrap_Struct(klass,NULL,frostbitten_header_deallocate, header);
}


void header_init() {
	c_frostbitten_header = rb_define_class_under(m_frostbitten, "Header", rb_cObject);
	rb_define_alloc_func(c_frostbitten_header, frostbitten_header_allocate);
	rb_define_attr(c_frostbitten_header, "sequence", 1, 1);
    rb_define_attr(c_frostbitten_header, "clientOrigin", 1, 1);
    rb_define_attr(c_frostbitten_header, "response", 1, 1);


    rb_define_method(c_frostbitten_header,"sequence=", frostbitten_header_set_sequence,1);
    rb_define_method(c_frostbitten_header,"sequence", frostbitten_header_get_sequence,0);
    
    rb_define_method(c_frostbitten_header,"is_client_origin=", frostbitten_header_set_clientOrigin, 1);
    rb_define_method(c_frostbitten_header,"is_client_origin?", frostbitten_header_get_clientOrigin, 0);
    


    rb_define_method(c_frostbitten_header, "read", frostbitten_header_read_from_io, 1);
    rb_define_method(c_frostbitten_header, "write", frostbitten_header_write_to_io, 1);
    rb_define_singleton_method(c_frostbitten_header, "read", frostbitten_header_cls_read_from_io, 1);

}
