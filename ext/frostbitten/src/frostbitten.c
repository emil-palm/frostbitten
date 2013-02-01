// Include the Ruby headers and goodies
#include "frostbitten.h"
#include "word.h"
#include "header.h"
#include "message.h"
VALUE m_frostbitten;
// // The initialization method for this module
void Init_frostbitten() {
	m_frostbitten = rb_define_module("Frostbitten");
	header_init();
 	message_init();
 	word_init();
}