#pragma once
#include <istream>

namespace io
{
	uint64_t read_variable_length_integer(std::istream& in);
}
