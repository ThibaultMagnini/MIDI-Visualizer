#include <cstdint>
#include <functional>
#include <istream>
#include <vector>


#include "primitives.h"

#pragma once
namespace midi
{
	struct CHUNK_HEADER
	{
		char id[4];
		uint32_t size;
	};

	void read_chunk_header(std::istream& ss, CHUNK_HEADER *header);
	std::string header_id(CHUNK_HEADER header);

	#pragma pack(push,1)
	struct MTHD
	{
		CHUNK_HEADER header;
		uint16_t type;
		uint16_t ntracks;
		uint16_t division;
	};
	#pragma pack(pop)

	void read_mthd(std::istream& ss, MTHD* header);

	/*==================================================================================================================================================================*/

	bool is_sysex_event(uint8_t byte);
	bool is_meta_event(uint8_t byte);
	bool is_midi_event(uint8_t byte);
	bool is_running_status(uint8_t byte);
	uint8_t extract_midi_event_type(uint8_t status);
	midi::Channel extract_midi_event_channel(uint8_t status);
	bool is_note_off(uint8_t status);
	bool is_note_on(uint8_t status);
	bool is_polyphonic_key_pressure(uint8_t status);
	bool is_control_change(uint8_t status);
	bool is_program_change(uint8_t status);
	bool is_channel_pressure(uint8_t status);
	bool is_pitch_wheel_change(uint8_t status);

	/*====================================================================================================================================================================*/


	class EventReceiver
	{
	public:
		virtual void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) = 0;
		virtual void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) = 0;
		virtual void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) = 0;
		virtual void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) = 0;
		virtual void program_change(Duration dt, Channel channel, Instrument program) = 0;
		virtual void channel_pressure(Duration dt, Channel channel, uint8_t pressure) = 0;
		virtual void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) = 0;
		virtual void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) = 0;
		virtual void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) = 0;
	};


	/*=========================================================================================================================================================================*/

	void read_mtrk(std::istream& in, EventReceiver& receiver);
	void runningstatus(midi::Channel lastchannel, uint8_t identifier, std::string lastevent, midi::EventReceiver& receiver, midi::Duration duration, std::istream& in);

	/*==========================================================================================================================================================================*/


	struct NOTE
	{
		NoteNumber note_number;
		Time start;
		Duration duration;
		uint8_t velo;
		Instrument instrument;

		NOTE(NoteNumber number, Time start_time, Duration duration, uint8_t velo, Instrument instrument)
		{
			this->note_number = number;
			this->duration = duration;
			this->velo = velo;
			this->instrument = instrument;
			this->start = start_time;
		}

		NOTE() {}
	};

	bool operator !=(const NOTE& x, const NOTE& y);
	bool operator ==(const NOTE& x, const NOTE& y);
	std::ostream& operator<<(std::ostream& stream, const NOTE& note);


	/*===========================================================================================================================================================================*/

	class ChannelNoteCollector : public EventReceiver
	{
		
	public:
		Channel channel;
		std::function<void(const NOTE&)> note_receiver;
		std::vector<NOTE> current_notes;
		Time start;
		Instrument instrument;
		
		ChannelNoteCollector(Channel channel, std::function<void(const NOTE&)> note_receiver) : channel(channel), note_receiver(note_receiver), current_notes(0), instrument(0) {}

		void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
		void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
		void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
		void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
		void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) override;
		void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) override;
		void program_change(Duration dt, Channel channel, Instrument program) override;
		void channel_pressure(Duration dt, Channel channel, uint8_t pressure) override;
		void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) override;
		NOTE* checknote(NoteNumber number, int& x);
		void update(Duration dt);
	};


	/*============================================================================================================================================================================*/

	class EventMulticaster : public EventReceiver
	{
		
	public:
		std::vector<std::shared_ptr<EventReceiver>> list;
		
		EventMulticaster(std::vector<std::shared_ptr<EventReceiver>> list) : list(std::move(list)) {}

		EventMulticaster() {}

		void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
		void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
		void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
		void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
		void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) override;
		void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) override;
		void program_change(Duration dt, Channel channel, Instrument program) override;
		void channel_pressure(Duration dt, Channel channel, uint8_t pressure) override;
		void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) override;
	};

	/*=========================================================================================================================================================================*/

	class NoteCollector : public EventReceiver
	{
	public:
		std::function<void(const NOTE&)> note_receiver;
		EventMulticaster caster;
		
		NoteCollector(std::function<void(const NOTE&)> note_receiver) : note_receiver(std::move(note_receiver))
		{
			std::vector<std::shared_ptr<EventReceiver>> yeet;
			for (int i = 0; i < 16; i++)
			{
				Channel channel(i);
				std::shared_ptr<ChannelNoteCollector> euh_inspiratie_voor_var_naam_is_op = std::make_shared<ChannelNoteCollector>(channel, this->note_receiver);
				yeet.push_back(euh_inspiratie_voor_var_naam_is_op);
			}
			caster = EventMulticaster(yeet);
		}
		
		void meta(Duration dt, uint8_t type, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
		void sysex(Duration dt, std::unique_ptr<uint8_t[]> data, uint64_t data_size) override;
		void note_on(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
		void note_off(Duration dt, Channel channel, NoteNumber note, uint8_t velocity) override;
		void polyphonic_key_pressure(Duration dt, Channel channel, NoteNumber note, uint8_t pressure) override;
		void control_change(Duration dt, Channel channel, uint8_t controller, uint8_t value) override;
		void program_change(Duration dt, Channel channel, Instrument program) override;
		void channel_pressure(Duration dt, Channel channel, uint8_t pressure) override;
		void pitch_wheel_change(Duration dt, Channel channel, uint16_t value) override;
	};

	/*================================================================================================================================================================================*/

	std::vector<NOTE> read_notes(std::istream& in);

}