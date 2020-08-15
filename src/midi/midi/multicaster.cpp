#include "midi/midi.h"


void midi::EventMulticaster::sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->sysex(dt,std::move(data),data_size);
	}
}

void midi::EventMulticaster::meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->meta(dt, type, std::move(data), data_size);
	}
}


void midi::EventMulticaster::note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->note_on(dt, channel, note, velocity);
	}
}

void midi::EventMulticaster::note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->note_off(dt, channel, note, velocity);
	}
}

void midi::EventMulticaster::polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->polyphonic_key_pressure(dt, channel, note, pressure);
	}
}

void midi::EventMulticaster::control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->control_change(dt, channel, controller, value);
	}
}

void midi::EventMulticaster::program_change(Duration dt, Channel channel, Instrument program)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->program_change(dt, channel, program);
	}
}

void midi::EventMulticaster::channel_pressure(Duration dt, Channel channel, uint8_t pressure)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->channel_pressure(dt, channel, pressure);
	}
}

void midi::EventMulticaster::pitch_wheel_change(Duration dt, Channel channel, uint16_t value)
{
	for (int i = 0; i < this->list.size(); i++)
	{
		this->list[i]->pitch_wheel_change(dt, channel, value);
	}
}





