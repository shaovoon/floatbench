// BenchmarkFloatConv.cpp : Defines the entry point for the console application.
//

#include <vector>
#include <iostream>
#include <iomanip>
#include <string>
#include <cstring>
#include <cmath>
#include <cassert>
#include <sstream>
#include <cstdlib>
#include <chrono>
#include <boost/lexical_cast.hpp>
#include <boost/spirit/include/qi.hpp>

typedef std::pair<const std::string, const double> pair_type;
typedef std::vector< pair_type > vector_type;

#ifdef WIN32

#pragma optimize("", off)
template <class T>
void do_not_optimize_away(T&& datum) {
	datum = datum;
}
#pragma optimize("", on)

#else
static void do_not_optimize_away(void* p) { 
    asm volatile("" : : "g"(p) : "memory");
}
#endif

void init(vector_type& vec);

#define MYASSERT(value, expected) 

//#define MYASSERT(value, expected) if(value != expected) { std::cerr << value << " and expected:" << expected << " are different" << std::endl; }

class timer
{
public: 
	timer() = default;
	void start_timing(const std::string& text_)
	{
		text = text_;
		begin = std::chrono::high_resolution_clock::now();
	}
	void stop_timing()
	{
		auto end = std::chrono::high_resolution_clock::now();
		auto dur = end - begin;
		auto ms = std::chrono::duration_cast<std::chrono::milliseconds>(dur).count();
		std::cout << std::setw(19) << text << ":" << std::setw(5) << ms << "ms" << std::endl;
	}

private:
	std::string text;
	std::chrono::steady_clock::time_point begin;
};

// http://crackprogramming.blogspot.sg/2012/10/implement-atof.html
double crack_atof(const char* num)
{
	if (!num || !*num)
		return 0;
	double integerPart = 0;
	double fractionPart = 0;
	int divisorForFraction = 1;
	int sign = 1;
	bool inFraction = false;
	/*Take care of +/- sign*/
	if (*num == '-')
	{
		++num;
		sign = -1;
	}
	else if (*num == '+')
	{
		++num;
	}
	while (*num != '\0')
	{
		if (*num >= '0' && *num <= '9')
		{
			if (inFraction)
			{
				/*See how are we converting a character to integer*/
				fractionPart = fractionPart * 10 + (*num - '0');
				divisorForFraction *= 10;
			}
			else
			{
				integerPart = integerPart * 10 + (*num - '0');
			}
		}
		else if (*num == '.')
		{
			if (inFraction)
				return sign * (integerPart + fractionPart / divisorForFraction);
			else
				inFraction = true;
		}
		else
		{
			return sign * (integerPart + fractionPart / divisorForFraction);
		}
		++num;
	}
	return sign * (integerPart + fractionPart / divisorForFraction);
}

#define white_space(c) ((c) == ' ' || (c) == '\t')
#define valid_digit(c) ((c) >= '0' && (c) <= '9')
// http://www.leapsecond.com/tools/fast_atof.c
double fast_atof(const char *p)
{
	int frac;
	double sign, value, scale;

	// Skip leading white space, if any.

	while (white_space(*p)) {
		p += 1;
	}

	// Get sign, if any.

	sign = 1.0;
	if (*p == '-') {
		sign = -1.0;
		p += 1;

	}
	else if (*p == '+') {
		p += 1;
	}

	// Get digits before decimal point or exponent, if any.

	for (value = 0.0; valid_digit(*p); p += 1) {
		value = value * 10.0 + (*p - '0');
	}

	// Get digits after decimal point, if any.

	if (*p == '.') {
		double pow10 = 10.0;
		p += 1;
		while (valid_digit(*p)) {
			value += (*p - '0') / pow10;
			pow10 *= 10.0;
			p += 1;
		}
	}

	// Handle exponent, if any.

	frac = 0;
	scale = 1.0;
	if ((*p == 'e') || (*p == 'E')) {
		unsigned int expon;

		// Get sign of exponent, if any.

		p += 1;
		if (*p == '-') {
			frac = 1;
			p += 1;

		}
		else if (*p == '+') {
			p += 1;
		}

		// Get digits of exponent, if any.

		for (expon = 0; valid_digit(*p); p += 1) {
			expon = expon * 10 + (*p - '0');
		}
		if (expon > 308) expon = 308;

		// Calculate scaling factor.

		while (expon >= 50) { scale *= 1E50; expon -= 50; }
		while (expon >= 8) { scale *= 1E8;  expon -= 8; }
		while (expon > 0) { scale *= 10.0; expon -= 1; }
	}

	// Return signed and scaled floating point result.

	return sign * (frac ? (value / scale) : (value * scale));
}
int main(int argc, char *argv [])
{
	const size_t MAX_LOOP = (argc == 2) ? atoi(argv[1]) : 100000;

	vector_type vec;
	init(vec);
	timer stopwatch;
	double d = 0.0;

	stopwatch.start_timing("atof");
	for (size_t k = 0; k<MAX_LOOP; ++k)
	{
		for (size_t j=0; j<vec.size(); ++j)
		{
			pair_type& pr = vec[j];
			d = std::atof(pr.first.c_str());
			do_not_optimize_away(&d);
			MYASSERT(d, pr.second);
		}
	}
	stopwatch.stop_timing();

	stopwatch.start_timing("lexical_cast");
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		for (size_t j=0; j<vec.size(); ++j)
		{
			pair_type& pr = vec[j];
			d = boost::lexical_cast<double>(pr.first.c_str());
			do_not_optimize_away(&d);
			MYASSERT(d, pr.second);
		}
	}
	stopwatch.stop_timing();

	stopwatch.start_timing("std::istringstream");
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		for (size_t i = 0; i<vec.size(); ++i)
		{
			pair_type& pr = vec[i];
			std::istringstream oss(pr.first);
			oss >> d;
			do_not_optimize_away(&d);
			MYASSERT(d, pr.second);
		}
	}
	stopwatch.stop_timing();

	stopwatch.start_timing("std::stod");
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		for (size_t i = 0; i<vec.size(); ++i)
		{
			pair_type& pr = vec[i];
			d = std::stod(pr.first, nullptr);
			do_not_optimize_away(&d);
			MYASSERT(d, pr.second);
		}
	}
	stopwatch.stop_timing();

	stopwatch.start_timing("crack_atof");
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		for (size_t j=0; j<vec.size(); ++j)
		{
			pair_type& pr = vec[j];
			d = crack_atof(pr.first.c_str());
			do_not_optimize_away(&d);
			MYASSERT(d, pr.second);
		}
	}
	stopwatch.stop_timing();

	stopwatch.start_timing("fast_atof");
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		for (size_t j=0; j<vec.size(); ++j)
		{
			pair_type& pr = vec[j];
			d = fast_atof(pr.first.c_str());
			do_not_optimize_away(&d);
			MYASSERT(d, pr.second);
		}
	}
	stopwatch.stop_timing();

	namespace qi = boost::spirit::qi;

	stopwatch.start_timing("boost_spirit");
	for (size_t k = 0; k < MAX_LOOP; ++k)
	{
		for (size_t j=0; j<vec.size(); ++j)
		{
			pair_type& pr = vec[j];
			d = fast_atof(pr.first.c_str());
			auto iter = pr.first.begin();
			qi::parse(iter, pr.first.end(), qi::double_, d);
			do_not_optimize_away(&d);
			MYASSERT(d, pr.second);
		}
	}
	stopwatch.stop_timing();

	std::cout << "Last float value: " << d << " <-- Ignore this" << std::endl;
	return 0;
}

void init(vector_type& vec)
{
	std::string float_str = "+12369";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "-25934";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "-47896.36";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "+532.102";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "4.5655";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "8.3658";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "-125.6900";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "-1236.2311";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "-5522.2389";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
	float_str = "-14.23";
	vec.push_back(std::make_pair(float_str, atof(float_str.c_str())));
}

