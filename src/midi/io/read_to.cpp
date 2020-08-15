#include "read.h"
#include <iostream>
#include "logging.h"
#include "vli.h"

template <typename T>
void io::read_to(std::istream& in, T* buffer, unsigned n)
{
	std::cout << sizeof(*buffer) << std::endl;
	in.read(reinterpret_cast<char*>(buffer), sizeof(*buffer)* n);
	//CHECK(in.read(reinterpret_cast<char*>(buffer), sizeof(*buffer)* n).fail()) << "FAILED READ";
}

template <typename T, typename std::enable_if<std::is_fundamental<T>::value, T>::type*>
T io::read(std::istream& in)
{
	T* buffer = new T[1];
	io::read_to(in, buffer, 1);
	return *buffer;
}

template <typename T>
std::unique_ptr<T[]> io::read_array(std::istream& in, unsigned n)
{
	std::unique_ptr<T[]> buffer = std::make_unique<T[]>(n);
	io::read_to(in, buffer.get(), n);
	return buffer;
}

uint64_t io::read_variable_length_integer(std::istream& in)
{
	uint8_t buffer = in.get();
	uint64_t result = 0;
	while ((buffer >> 7) == 0x00000001)
	{
		result = result << 7 | ((buffer) & 0b01111111);
		buffer = in.get();
	}
	result = result << 7 | (buffer & 0b01111111);
	return result;
}



	
	