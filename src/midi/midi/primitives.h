#pragma once
#include "util/tagged.h"

namespace midi
{
	struct __declspec(empty_bases) Channel : tagged<uint8_t, Channel>, equality<Channel>, show_value<Channel, int>
	{
		using tagged::tagged;
	};

	struct __declspec(empty_bases) Instrument : tagged<uint8_t, Instrument>, equality<Instrument>, show_value<Instrument, int>
	{
		using tagged::tagged;
	};

	struct __declspec(empty_bases) NoteNumber : tagged<uint8_t, NoteNumber>, ordered<NoteNumber>, show_value<NoteNumber, int>
	{
		using tagged::tagged;
	};

	struct __declspec(empty_bases) Time : tagged<uint64_t, Time>, ordered<Time>, show_value<Time, int>
	{
		using tagged::tagged;
	};

	struct __declspec(empty_bases) Duration : tagged<uint64_t, Duration>, ordered<Duration>, show_value<Duration, int>
	{
		using tagged::tagged;
	};

	Duration operator +(const Duration& x, const Duration& y);
	Time operator +(const midi::Time& x, const midi::Duration& y);
	Time operator +(const midi::Duration& x, const midi::Time& y);
	Duration operator -(const midi::Time& x, const midi::Time& y);
	Duration operator -(const midi::Duration& x, const midi::Duration& y);
	Time& operator +=(midi::Time& x, const midi::Duration& y);
	Duration& operator +=(midi::Duration& x, const midi::Duration& y);
	Duration& operator -=(midi::Duration& x, const midi::Duration& y);

}



