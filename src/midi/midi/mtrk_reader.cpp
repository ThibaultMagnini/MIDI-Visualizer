#include "midi/midi.h"
#include "io/vli.h"

void midi::read_mtrk(std::istream& in, EventReceiver& receiver)
{
	CHUNK_HEADER header; 
	midi::read_chunk_header(in, &header);
	std::string lastevent;
	Channel lastchannel;

	if (midi::header_id(header) == "MTrk")
	{
		bool end_of_track_reached = false;
		
		while (!end_of_track_reached) {

			midi::Duration duration = Duration(io::read_variable_length_integer(in));
			uint8_t identifier = in.get();
			
			if (!midi::is_running_status(identifier))
			{
				if (midi::is_meta_event(identifier))
				{
					uint8_t event_type = in.get();
					if (event_type == 0x2f)
					{
						uint64_t data = io::read_variable_length_integer(in);
						std::unique_ptr<uint8_t[]> empty_data = std::make_unique<uint8_t[]>(data);
						receiver.meta(duration, event_type, std::move(empty_data), data);
						end_of_track_reached = true;
					}
					else
					{
						uint64_t data_size = io::read_variable_length_integer(in);
						std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(data_size);
						in.read(reinterpret_cast<char*>(data.get()), sizeof(char) * data_size);
						receiver.meta(duration, event_type, std::move(data), data_size);
						lastevent = "meta";
					}
				}
				else if (midi::is_sysex_event(identifier))
				{
					uint64_t data_size = io::read_variable_length_integer(in);
					std::unique_ptr<uint8_t[]> data = std::make_unique<uint8_t[]>(data_size);
					in.read(reinterpret_cast<char*>(data.get()), sizeof(char)* data_size);
					receiver.sysex(duration, std::move(data), data_size);
					lastevent = "sysex";
				}
				else if (midi::is_note_on(midi::extract_midi_event_type(identifier)))
				{
					Channel channel = Channel(midi::extract_midi_event_channel(identifier));
					
					uint8_t note = in.get();
					NoteNumber notenr = NoteNumber(note);
					
					uint8_t velocity = in.get();
					
					receiver.note_on(duration, channel, notenr, velocity);
					lastevent = "noteon";
					lastchannel = channel;
				}
				else if (midi::is_note_off(midi::extract_midi_event_type(identifier)))
				{
					Channel channel = Channel(midi::extract_midi_event_channel(identifier));
					
					uint8_t note = in.get();
					NoteNumber notenr = NoteNumber(note);
					
					uint8_t velocity = in.get();
					
					receiver.note_off(duration, channel, notenr, velocity);
					lastevent = "noteoff";
					lastchannel = channel;
				}
				else if (midi::is_polyphonic_key_pressure(midi::extract_midi_event_type(identifier)))
				{
					Channel channel = Channel(midi::extract_midi_event_channel(identifier));

					uint8_t note = in.get();
					NoteNumber notenr = NoteNumber(note);

					uint8_t velocity = in.get();

					receiver.polyphonic_key_pressure(duration, channel, notenr, velocity);
					lastevent = "poly";
					lastchannel = channel;
				}
				else if (midi::is_control_change(midi::extract_midi_event_type(identifier)))
				{
					Channel channel = Channel(midi::extract_midi_event_channel(identifier));

					uint8_t controller = in.get();

					uint8_t value = in.get();

					receiver.control_change(duration, channel, controller, value);

					lastevent = "control";
					lastchannel = channel;
				}
				else if (midi::is_program_change(midi::extract_midi_event_type(identifier)))
				{
					Channel status = Channel(midi::extract_midi_event_channel(identifier));

					uint8_t ins = in.get();
					Instrument instrument = Instrument(ins);

					receiver.program_change(duration, status, instrument);
					lastevent = "programchange";
					lastchannel = status;
				}
				else if (midi::is_channel_pressure(midi::extract_midi_event_type(identifier)))
				{
					Channel status = Channel(midi::extract_midi_event_channel(identifier));

					uint8_t pressure = in.get();

					receiver.channel_pressure(duration, status, pressure);
					lastevent = "channelp";
					lastchannel = status;
				}
				else if (midi::is_pitch_wheel_change(midi::extract_midi_event_type(identifier)))
				{
					Channel status = Channel(midi::extract_midi_event_channel(identifier));
					
					uint8_t lower7 = in.get();
					uint8_t upper7 = in.get();

					lower7 = lower7 & 0b01111111;
					upper7 = upper7 & 0b01111111;
					
					uint16_t result = (upper7 << 7) | lower7;

					receiver.pitch_wheel_change(duration, status, result);
					lastevent = "pitchwheel";
					lastchannel = status;
				}
			}
			else
			{
				midi::runningstatus(lastchannel, identifier, lastevent, receiver, duration, in); 
			}
		}
	}
}

void midi::runningstatus(midi::Channel lastchannel, uint8_t identifier, std::string lastevent, midi::EventReceiver& receiver, midi::Duration duration, std::istream& in)
{
	if (lastevent == "noteon")
	{
		midi::NoteNumber note = midi::NoteNumber(identifier);
		uint8_t velo = in.get();
		receiver.note_on(duration, lastchannel, note, velo);
	}
	else if (lastevent == "noteoff")
	{
		midi::NoteNumber note = midi::NoteNumber(identifier);
		uint8_t velo = in.get();
		receiver.note_off(duration, lastchannel, note, velo);
	}
	else if (lastevent == "control")
	{
		uint8_t controller = identifier;
		uint8_t value = in.get();
		receiver.control_change(duration, lastchannel, controller, value);
	}
	else if (lastevent == "poly")
	{
		midi::NoteNumber note = midi::NoteNumber(identifier);
		uint8_t pressure = in.get();
		receiver.polyphonic_key_pressure(duration, lastchannel, note, pressure);
	}
	else if (lastevent == "programchange")
	{
		midi::Instrument instrument = midi::Instrument(identifier);
		receiver.program_change(duration, lastchannel, instrument);
	}
	else if (lastevent == "channelp")
	{
		uint8_t pressure = identifier;
		receiver.channel_pressure(duration, lastchannel, pressure);
	}
	else if (lastevent == "pitchwheel")
	{
		uint8_t lower7 = identifier;
		uint8_t upper7 = in.get();

		lower7 = lower7 & 0b01111111;
		upper7 = upper7 & 0b01111111;

		uint16_t result = (upper7 << 7) | lower7;
		receiver.pitch_wheel_change(duration, lastchannel, result);
	}
}