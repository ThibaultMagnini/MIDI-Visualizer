#include "midi/midi.h"

void midi::ChannelNoteCollector::sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	this->start += dt;
}

void midi::ChannelNoteCollector::note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	if (this->channel == channel)
	{
		if (velocity != 0)
		{
			int x = 0;
			NOTE* note1 = checknote(note, x);
			if (note1 == nullptr)
			{
				this->start += dt;
				NOTE noot;
				noot.start = start;
				noot.velo = velocity;
				noot.note_number = note;
				noot.instrument = this->instrument;
				for (int i = 0; i < this->current_notes.size(); i++)
				{
					this->current_notes[i].duration += dt;
				}
				this->current_notes.push_back(noot);
			}
			else
			{
				this->note_off(dt, channel, note, velocity);
				NOTE noot1;
				noot1.velo = velocity;
				noot1.note_number = note;
				noot1.start = start;
				this->current_notes.push_back(noot1);
			}
		}
		else
		{
			this->note_off(dt, channel, note, velocity);
		}
	} else
	{
		update(dt);
	}	
}

void midi::ChannelNoteCollector::note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	if (this->channel == channel)
	{
		for (int i = 0; i < this->current_notes.size(); i++)
		{
			this->current_notes[i].duration += dt;
		}
		this->start += dt;
		int x = 0;
		NOTE* noot = checknote(note, x);
		if (noot != nullptr)
		{
			this->note_receiver(*noot);
			this->current_notes.erase(this->current_notes.begin() + x);
		}
	} else
	{
		update(dt);
	}
}

void midi::ChannelNoteCollector::polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure)
{
	this->start += dt;
}

void midi::ChannelNoteCollector::control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value)
{
	this->start += dt;
}

void midi::ChannelNoteCollector::program_change(Duration dt, Channel channel, Instrument program)
{
	if (this->channel == channel)
	{
		this->instrument = program;
	} else
	{
		update(dt);
	}
}

void midi::ChannelNoteCollector::channel_pressure(Duration dt, Channel channel, uint8_t pressure)
{
	this->start += dt;
}

void midi::ChannelNoteCollector::pitch_wheel_change(Duration dt, Channel channel, uint16_t value)
{
	this->start += dt;
}

void midi::ChannelNoteCollector::meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	this->start += dt;
}


midi::NOTE* midi::ChannelNoteCollector::checknote(NoteNumber number, int& x)
{
	
	for (unsigned i = 0; i < this->current_notes.size(); i++)
	{
		if (this->current_notes[i].note_number == number)
		{
			x = i;
			return &this->current_notes[i];
		}
	}
	return nullptr;
}

void midi::ChannelNoteCollector::update(Duration dt)
{
	this->start += dt;
	if (!this->current_notes.empty())
	{
		for (int i = 0; i < this->current_notes.size(); i++)
		{
			this->current_notes[i].duration += dt;
		}
	}
}









