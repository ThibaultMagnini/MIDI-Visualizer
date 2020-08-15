#include "midi/midi.h"

bool midi::is_sysex_event(uint8_t byte)
{
	return byte == 0xF0 || byte == 0xF7;
}

bool midi::is_meta_event(uint8_t byte)
{
	return byte == 0xFF || byte == 0x2F;
}

bool midi::is_midi_event(uint8_t byte)
{
	//return byte == 0x80 || byte == 0x81 || byte == 0x8F || byte == 0x95 || byte == 0xA3 || byte == 0xB8 || byte == 0xC3 || byte == 0xD1 || byte == 0xE7;
	byte = byte >> 4;
	return byte == 0x8 || byte ==0x9 || byte == 0xA || byte == 0xB || byte == 0xC || byte == 0xD || byte == 0xE;
}

bool midi::is_running_status(uint8_t byte)
{
	return ((byte) & 0b10000000) == 0b0;
}


uint8_t midi::extract_midi_event_type(uint8_t status)
{
	 return ((status) & 0b11110000) >> 4;
}

midi::Channel midi::extract_midi_event_channel(uint8_t status)
{
	uint8_t a = status & 0b00001111;
	Channel result = Channel(a);
	return result;
}

bool midi::is_note_off(uint8_t status)
{
	return status == 0x08;
}

bool midi::is_note_on(uint8_t status)
{
	return status == 0x09;
}

bool midi::is_polyphonic_key_pressure(uint8_t status)
{
	return status == 0x0A;
}

bool midi::is_control_change(uint8_t status)
{
	return status == 0x0B;
}

bool midi::is_program_change(uint8_t status)
{
	return status == 0x0C;
}

bool midi::is_channel_pressure(uint8_t status)
{
	return status == 0x0D;
}

bool midi::is_pitch_wheel_change(uint8_t status)
{
	return status == 0x0E;
}
