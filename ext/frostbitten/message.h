#include "frostbitten.h"
#include "word.h"
#include "header.h"

#ifndef message_h
#define message_h
void message_init();
extern VALUE c_frostbitten_message;
// Frostbitten::Message
typedef struct _fb_message {
	VALUE header; // Frostbitten::Header
	VALUE words; // Array
} fb_message;
void frostbitten_message_deallocate(fb_message *message);
VALUE frostbitten_message_allocate (VALUE klass);
char** frostbitten_message_generate(fb_message *message, uint32_t *message_size);
#endif
