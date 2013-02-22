#include "frostbitten.h"
#ifndef packet_h
#define packet_h
typedef struct _fb_packet_buffer {
	uint32_t header;
	uint32_t packetSize;
	uint32_t wordCount;
} fb_packet_buffer;
fb_packet_buffer* read_buffer_from_io(FILE *fp);
bool is_packet_valid(fb_packet_buffer *buffer);
#endif