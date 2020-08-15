#include "midi/midi.h"


std::vector<midi::NOTE> midi::read_notes(std::istream& in)
{
	std::vector<NOTE> notes;
	MTHD mthd;
	midi::read_mthd(in, &mthd);
	for (int i = 0; i < mthd.ntracks; i++)
	{
		NoteCollector collector([&notes](const midi::NOTE& note) { notes.push_back(note); });
		midi::read_mtrk(in, collector);
	}
	return notes;
}
