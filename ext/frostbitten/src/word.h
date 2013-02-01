#include "frostbitten.h"

#ifndef word_h
#define word_h

extern VALUE c_frostbitten_word;
void word_init();
typedef struct _fb_word {
	uint32_t len;
	char str[INT_MAX];
	char term;
} fb_word;

void frostbitten_word_deallocate(fb_word *word);
VALUE frostbitten_word_allocate (VALUE klass);

#endif