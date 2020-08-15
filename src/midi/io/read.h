#include "istream"

#pragma once

namespace io
{
	template<typename T>
	void read_to(std::istream& in, T* buffer, unsigned n = 1);


	template<typename T, typename std::enable_if<std::is_fundamental<T>::value, T>::type* = nullptr>
	T read(std::istream& in);

	template<typename T>
	std::unique_ptr<T[]> read_array(std::istream& in, unsigned n);
	
}
