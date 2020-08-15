#include "io/endianness.h"

void io::switch_endianness(uint16_t* n)
{
	uint16_t a = ((*n) & 0xFF00) >> 8;
	uint16_t b = ((*n) & 0x00FF) >> 0;

	b <<= 8;
	a <<= 0;

	*n = (b | a);
}

void io::switch_endianness(uint32_t* n)
{
	uint32_t a = ((*n) & 0x000000FF) >> 0;
	uint32_t b = ((*n) & 0x0000FF00) >> 8;
	uint32_t c = ((*n) & 0x00FF0000) >> 16;
	uint32_t d = ((*n) & 0xFF000000) >> 24;

	a <<= 24;
	b <<= 16;
	c <<= 8;
	d <<= 0;

	*n = (a | b | c | d);
}

void io::switch_endianness(uint64_t* n)
{
	uint64_t a = ((*n) & 0x00000000000000FF) >> 0;
	uint64_t b = ((*n) & 0x000000000000FF00) >> 8;
	uint64_t c = ((*n) & 0x0000000000FF0000) >> 16;
	uint64_t d = ((*n) & 0x00000000FF000000) >> 24;
	uint64_t e = ((*n) & 0x000000FF00000000) >> 32;
	uint64_t f = ((*n) & 0x0000FF0000000000) >> 40;
	uint64_t g = ((*n) & 0x00FF000000000000) >> 48;
	uint64_t h = ((*n) & 0xFF00000000000000) >> 56;
	
	a <<= 56;
	b <<= 48;
	c <<= 40;
	d <<= 32;
	e <<= 24;
	f <<= 16;
	g <<= 8;
	h <<= 0;

	*n = (a | b | c | d | e | f | g | h);
}


