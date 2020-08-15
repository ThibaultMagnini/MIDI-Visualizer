#include "Catch.h"
#include "midi.h"
#include "primitives.h"

midi::Duration midi::operator +(const midi::Duration& x, const midi::Duration& y)
{
	return midi::Duration(value(x) + value(y));
}

midi::Time midi::operator +(const midi::Time& x , const midi::Duration& y)
{
	return midi::Time(value(x) + value(y));
}

midi::Time midi::operator +(const midi::Duration& x, const midi::Time& y)
{
	return midi::Time(value(x) + value(y));
}

midi::Duration midi::operator -(const midi::Time& x, const midi::Time& y)
{
	return midi::Duration(value(x) - value(y));
}

midi::Duration midi::operator -(const midi::Duration& x, const midi::Duration& y)
{
	return midi::Duration(value(x) - value(y));
}

midi::Time& midi::operator +=(midi::Time& x, const midi::Duration& y)
{
	return x = x + y;
}

midi::Duration& midi::operator +=(midi::Duration& x, const midi::Duration& y)
{
	return x = x + y;
}

midi::Duration& midi::operator -=(midi::Duration& x, const midi::Duration& y)
{
	return x = x - y;
}


bool midi::operator!=(const NOTE& x, const NOTE& y)
{
	return !(x == y);
}

bool midi::operator==(const NOTE& x, const NOTE& y)
{
	return x.velo == y.velo && x.duration == y.duration && x.instrument == y.instrument && x.note_number == y.note_number && x.start == y.start;
}

std::ostream& midi::operator<<(std::ostream& stream, const midi::NOTE& note)
{
	stream << "Note(number=" << note.note_number << ",start=" << note.start<< ",duration=" << note.duration << ",instrument=" << note.instrument << ")";
	return stream;
}