#include "midi/midi.h"


void midi::NoteCollector::sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	caster.sysex(dt, std::move(data), data_size);
}

void midi::NoteCollector::note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	caster.note_on(dt, channel, note, velocity);
}

void midi::NoteCollector::note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity)
{
	caster.note_off(dt, channel, note, velocity);
}

void midi::NoteCollector::polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure)
{
	caster.polyphonic_key_pressure(dt, channel, note, pressure);
}

void midi::NoteCollector::control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value)
{
	caster.control_change(dt, channel, controller, value);
}

void midi::NoteCollector::program_change(Duration dt, Channel channel, Instrument program)
{
	caster.program_change(dt, channel, program);
}

void midi::NoteCollector::channel_pressure(Duration dt, Channel channel, uint8_t pressure)
{
	caster.channel_pressure(dt, channel, pressure);
}

void midi::NoteCollector::pitch_wheel_change(Duration dt, Channel channel, uint16_t value)
{
	caster.pitch_wheel_change(dt, channel, value);
}

void midi::NoteCollector::meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size)
{
	caster.meta(dt, type, std::move(data), data_size);
}







