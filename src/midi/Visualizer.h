#include <iostream>

#pragma once


namespace visu 
{

	struct Visualizer 
	{
		unsigned width;
		unsigned step;
		unsigned scale;
		unsigned heightPixel;
		std::string filename;
		std::string pattern;

		Visualizer() : width(0), step(1), scale(100), heightPixel(16) {}
	};
}