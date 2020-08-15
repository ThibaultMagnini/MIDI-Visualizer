#ifndef TEST_BUILD
#include <iomanip>
#include <iostream> 
#include <sstream> 
#include <fstream>
#include "imaging/bmp-format.h"
#include "midi/midi.h"
#include "shell/command-line-parser.h"
#include "Visualizer.h"
#include <regex>
#include "windows.h" 



void drawRect(int note_number, int start, unsigned thickness, int duration, imaging::Bitmap& bitmap, int index)
{
	for (int y = note_number * thickness; y < note_number * thickness + thickness; y++)
	{
		for (int x = start; x < start + duration ; x++)
		{
			
			bitmap[Position(x, y)] = imaging::colors::red();
			
			if (note_number % 7 == 0)
			{
				bitmap[Position(x, y)] = imaging::colors::red();
			}
			else if (note_number % 7 == 1)
			{
				bitmap[Position(x, y)] = imaging::colors::orange();
			}
			else if (note_number % 7 == 2)
			{
				bitmap[Position(x, y)] = imaging::colors::yellow();
			}
			else if (note_number % 7 == 3)
			{
				bitmap[Position(x, y)] = imaging::colors::green();
			}
			else if (note_number % 7 == 4)
			{
				bitmap[Position(x, y)] = imaging::colors::cyan();
			}
			else if (note_number % 7 == 5)
			{
				bitmap[Position(x, y)] = imaging::colors::blue();
			}
			else if (note_number % 7 == 6)
			{
				bitmap[Position(x, y)] = imaging::colors::magenta();
			}

		}
	}

}

void getMinMaxNote(std::vector<midi::NOTE>& notes, int* min, int* max)
{
	for (midi::NOTE n : notes)
	{
		if (value(n.note_number) < *min)
		{
			*min = value(n.note_number);
		}
		if (value(n.note_number) > *max)
		{
			*max = value(n.note_number);
		}
	}
}


std::string numFormatter(int width, int number)
{
	std::stringstream a;
	a << std::setfill('0') << std::setw(width) << number;
	return a.str();
}


std::vector<midi::NOTE> readFile(std::string path)
{
	char filename[200];
	strcpy(filename, path.c_str());
	std::ifstream in(filename, std::ios::binary);
	return midi::read_notes(in);
}

void slice(imaging::Bitmap bitmap, int counter, int y, int width, int step, std::string pattern)
{
	int total = (bitmap.width() / step);
	float progress = 0.0;
	for (int i = 0; i < bitmap.width(); i += 1)
	{
		if (i * step < bitmap.width() - width) {
			std::shared_ptr<imaging::Bitmap> yeet = bitmap.slice(i * step, y, width, bitmap.height());
			std::string filename = std::regex_replace(pattern, std::regex("%d"), numFormatter(5, counter));
			imaging::save_as_bmp(filename, *yeet);
			counter++;
			

			std::cout << "[";
			int pos = total * (float)(progress);
			for (int i = 0; i < total; i++) {
				if (i < pos) std::cout << "=";
				else if (i == pos) std::cout << ">";
				else std::cout << " ";
			}
			std::cout << "] " << int(progress * 100) << " %\r";
			std::cout.flush();	
			progress += (float)1 / total;
		}
	}
}

midi::NOTE getLongestNote(std::vector<midi::NOTE>& notes) {
	midi::NOTE result;
	for (int i = 0; i < notes.size(); i++) {
		if (notes[i].start + notes[i].duration > result.start + result.duration) {
			result = notes[i];
		}
	}
	return result;
}

void drawBitmap(visu::Visualizer* visualizer) 
{
	std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Do you want to develop an app?!! ~Glootie" << std::endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Visualizing: " << visualizer->filename << std::endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Be aware, if you are visualizing a Long midi file, a broken loading bar will show! Ignore it please :D" << std::endl;
	std::cout << "If its a shorter midi file, enjoy the progress bar :D" << std::endl;
	std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
	std::cout << "Reading in notes..." << std::endl;
	std::vector<midi::NOTE> notes = readFile(visualizer->filename);
	std::cout << "The notes are in!" << std::endl;

	int min = 1000;
	int max = 0;

	float scale = (float)visualizer->scale / 100.0;

	getMinMaxNote(notes, &min, &max);

	midi::NOTE a = getLongestNote(notes);

	int height = ((max - min) * visualizer->heightPixel) + visualizer->heightPixel * 2;

	std::cout << "Creating the bitmap..." << std::endl;
	std::cout << "Scaling with a scale of " << visualizer->scale << "%..." << std::endl;
	imaging::Bitmap bitmap((value(a.start + a.duration) / 10) * scale, height);

	bitmap.clear(imaging::Color(255, 182, 135));

	numFormatter(5, 2);

	std::cout << "Drawing the bitmap..." << std::endl;
	for (int i = 0; i < notes.size(); i++)
	{
		midi::NOTE n = notes[i];
		drawRect(max - value(n.note_number), (value(n.start) / 10) * scale, visualizer->heightPixel, (value(n.duration) / 10) * scale, bitmap, i);
	}

	int counter = 0;

	if (visualizer->width == 0) {
		std::cout << "Saving the bitmap..." << std::endl;
		std::string filename = std::regex_replace(visualizer->pattern, std::regex("%d"), "00000");
		imaging::save_as_bmp(filename, bitmap);
		std::cout << "Succes!" << std::endl;
	}
	else {
		std::cout << "Slicing the bitmap with width: " << visualizer->width << ", stepsize: " << visualizer->step << "..." << std::endl;
		slice(bitmap, counter, 0, visualizer->width, visualizer->step, visualizer->pattern);
		std::cout << "--------------------------------------------------------------------------------------------------------------------" << std::endl;
		std::cout << "I turned myself into a pickle, Morty *burp* ! I'm Pickle Ri-i-i-ick! ~Rick Sanchez C-137 AKA Pickle Rick" << std::endl;
	}
}


int main(int argn, char** argv)
{
	shell::CommandLineParser parser;
	visu::Visualizer visualizer;
	bool isWaarde = false;
	bool isFilename = false;

	for (int i = 1; i < argn; i++) {
		std::string ar(argv[i]);

		if (ar == "-w") 
		{
			parser.add_argument(std::string(argv[i]), &visualizer.width);
			isWaarde = true;
		}
		else if (ar == "-d")
		{
			parser.add_argument(std::string(argv[i]), &visualizer.step);
			isWaarde = true;
		}
		else if (ar == "-s")
		{
			parser.add_argument(std::string(argv[i]), &visualizer.scale);
			isWaarde = true;
		}
		else if (ar == "-h")
		{
			parser.add_argument(std::string(argv[i]), &visualizer.heightPixel);
			isWaarde = true;
		} 
		else if (!isWaarde && !isFilename)
		{
			visualizer.filename = argv[i];
			isFilename = true;
		}
		else if (!isWaarde && isFilename) 
		{
			visualizer.pattern = argv[i];
		}
		else if (isWaarde) 
		{
			isWaarde = false;
		}

	}
	parser.process(argn, argv);


	drawBitmap(&visualizer);
}

#endif
