/**
 * @file str_example.cpp
 *
 * @brief An example to show the use of str formatting
 *
 * @author Dahua Lin
 */

#include "../light_test/str_template.h"
#include <iostream>

using namespace ltest;

struct Record
{
	int index;
	std::string name;
	int score;

	Record(int i, const char *n, int s)
	: index(i), name(n), score(s) { }
};


struct RecordFormatter
{
	const Record& rec;

	explicit RecordFormatter(const Record& r) : rec(r) { }

	std::string operator() (const char* vname, const char *fmt=0) const
	{
		if (str_eq(vname, "index"))
		{
			return sformat(rec.index, fmt ? fmt : "%d");
		}
		else if (str_eq(vname, "name"))
		{
			return fmt ? sformat(rec.name.c_str(), fmt) : rec.name.c_str();
		}
		else if (str_eq(vname, "score"))
		{
			return sformat(rec.score, fmt ? fmt : "%d");
		}
		else
		{
			return "####";
		}
	}
};


RecordFormatter fmt(const Record& rec)
{
	return RecordFormatter(rec);
}

int main(int argc, char *argv[])
{
	Record records[5] = {
			Record(1, "John", 90),
			Record(2, "Mike", 100),
			Record(3, "Alice", 85),
			Record(4, "Bob",  70),
			Record(5, "David", 88)
	};

	size_t n = sizeof(records) / sizeof(records[0]);

	str_template templ("Record [{{index : %2d}}]:  {{ name: %-8s }} ( {{ score : %3d }} )");

	template_channel channel(std::cout);
	channel << templ;

	for (size_t i = 0; i < n; ++i)
	{
		channel << fmt(records[i]) << "\n";
	}
}




