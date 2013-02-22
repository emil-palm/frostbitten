#include "packet.h"

fb_packet_buffer* read_buffer_from_io(FILE *fp) {
	fb_packet_buffer *buffer = malloc(sizeof(fb_packet_buffer));
	fread((void*)&buffer->header, sizeof(uint32_t), 1, fp);
	fread((void*)&buffer->packetSize, sizeof(uint32_t), 1, fp);
	fread((void*)&buffer->wordCount, sizeof(uint32_t), 1, fp);
	return buffer;
}

bool is_packet_valid(fb_packet_buffer *buffer) {
	if ( !&buffer->header || !&buffer->packetSize ) {
		return false;
	}

	if ( buffer->packetSize > 16384 ) {
		return false;
	}
	return true;
}