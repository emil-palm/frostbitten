#include "frostbitten.h"

#ifndef header_h
#define header_h

extern VALUE c_frostbitten_header;

typedef struct _fb_header {
	uint32_t sequence;
	bool clientOrigin; // 1 == client, 0 == server
	bool response;
} fb_header;

void header_init();

VALUE frostbitten_header_get_sequence(VALUE self);
VALUE frostbitten_header_set_sequence(VALUE self, VALUE seq);
VALUE frostbitten_header_get_clientOrigin(VALUE self);
VALUE frostbitten_header_set_clientOrigin(VALUE self, VALUE clientOrigin);
uint32_t frostbitten_header_package(fb_header *header);
void frostbitten_header_unpack(fb_header *header, uint32_t val);

VALUE frostbitten_header_cls_read_from_io(VALUE self, VALUE io);
VALUE frostbitten_header_read_from_io(VALUE self, VALUE io);
VALUE frostbitten_header_write_to_io(VALUE self, VALUE io);
// VALUE frostbitten_header_test_write(VALUE self);
// VALUE frostbitten_header_test_read(VALUE self);
VALUE frostbitten_header_format(VALUE self);
void frostbitten_header_deallocate(fb_header *header);
VALUE frostbitten_header_allocate (VALUE klass);

#endif