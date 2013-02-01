#include "frostbitten.h"
#include "word.h"
#include "header.h"

#ifndef message_h
#define message_h
void message_init();
extern VALUE c_frostbitten_message;
// Frostbitten::Message
typedef struct _fb_message {
	fb_header *header;
	uint32_t packetSize;
	uint32_t wordCount;
	fb_word *words[];
} fb_message;
void frostbitten_message_deallocate(fb_message *message);
VALUE frostbitten_message_allocate (VALUE klass);
#endif
