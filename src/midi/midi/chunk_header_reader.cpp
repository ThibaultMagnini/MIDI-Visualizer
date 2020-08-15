#include "midi/midi.h"
#include "io/read.h"
#include "io/endianness.h"

void midi::read_chunk_header(std::istream& ss, CHUNK_HEADER *header)
{
	ss.read(reinterpret_cast<char*>(header), sizeof(*header));
	io::switch_endianness(&(header->size));
}

std::string midi::header_id(CHUNK_HEADER header)
{
	return std::string(header.id, 4);
}

void midi::read_mthd(std::istream& ss, MTHD* header)
{
	ss.read(reinterpret_cast<char*>(header), sizeof(*header));
	io::switch_endianness(&(header->division));
	io::switch_endianness(&(header->ntracks));
	io::switch_endianness(&(header->type));
	io::switch_endianness(&(header->header.size));
}
