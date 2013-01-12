/**
 * @file example1.cpp
 *
 * A simple example to demonstrate the use of light-test framework
 *
 * @author Dahua Lin
 */


#include "../light_test/tests.h"
#include "../light_test/std_test_mon.h"

#include <stdexcept>
#include <valarray>

using namespace ltest;

LTEST_INIT_AUTOSUITE

template<typename T>
struct NumPair
{
	T a, b;

	NumPair() : a(0), b(0) { }

	NumPair(T a_, T b_) : a(a_), b(b_) { }

	NumPair operator + (const NumPair& r) const
	{
		return NumPair(a + r.a, b + r.b);
	}

	NumPair operator - (const NumPair& r) const
	{
		return NumPair(a - r.a, b + r.b);  // make a typo here on purpose
	}

	bool operator == (const NumPair& r) const
	{
		return a == r.a && b == r.b;
	}

	bool operator != (const NumPair& r) const
	{
		return !(operator == (r));
	}
};


template<typename T>
struct NumTriple
{
	T a, b, c;

	NumTriple() : a(0), b(0), c(0) { }

	NumTriple(T a_, T b_, T c_) : a(a_), b(b_), c(c_) { }

	NumTriple operator + (const NumTriple& r) const
	{
		return NumTriple(a + r.a, b + r.b, c + r.c);
	}

	NumTriple operator - (const NumTriple& r) const
	{
		return NumTriple(a - r.a, b - r.b, c - r.c);
	}

	bool operator == (const NumTriple& r) const
	{
		return a == r.a && b == r.b && c == r.c;
	}

	bool operator != (const NumTriple& r) const
	{
		return !(operator == (r));
	}

	void RaiseError()
	{
		throw std::runtime_error("NumTriple Error");
	}
};


template<typename T> struct type_names;

template<> struct type_names<float>
{
	static const char *get() { return "float"; }
};

template<> struct type_names<double>
{
	static const char *get() { return "double"; }
};

#define MAX_TNAME_LEN 64


template<typename T>
class my_test_base : public test_case
{
	char m_name[MAX_TNAME_LEN];

public:
	my_test_base(const char *name)
	{
#if (defined(_WIN32) || defined(_WIN64))
		::sprintf_s(m_name, MAX_TNAME_LEN, "%s [%s]", name, type_names<T>::get());
#else
		std::sprintf(m_name, "%s [%s]", name, type_names<T>::get());
#endif
	}

	const char *name() const
	{
		return m_name;
	}
};


template<typename T>
class numpair_constructs : public my_test_base<T>
{
public:
	numpair_constructs() : my_test_base<T>("numpair_constructs") { }

	void run()
	{
		NumPair<T> x;
		ASSERT_EQ(x, NumPair<T>(0, 0));
	}
};

template<typename T>
class numpair_arith : public my_test_base<T>
{
public:
	numpair_arith() : my_test_base<T>("numpair_arith") { }

	void run()
	{
		NumPair<T> x(5, 6);
		NumPair<T> y(1, 2);

		ASSERT_EQ(x + y, NumPair<T>(6, 8));
		ASSERT_EQ(x - y, NumPair<T>(4, 4));
	}
};

template<typename T>
class numtriple_constructs : public my_test_base<T>
{
public:
	numtriple_constructs() : my_test_base<T>("numtriple_constructs") { }

	void run()
	{
		NumTriple<T> x;
		ASSERT_EQ(x, NumTriple<T>(0, 0, 0));
	}
};

template<typename T>
class numtriple_arith : public my_test_base<T>
{
public:
	numtriple_arith() : my_test_base<T>("numtriple_arith") { }

	void run()
	{
		NumTriple<T> x(5, 6, 8);
		NumTriple<T> y(1, 2, 3);

		ASSERT_EQ(x + y, NumTriple<T>(6, 8, 11));
		ASSERT_EQ(x - y, NumTriple<T>(4, 4, 5));
	}
};

template<typename T>
class numtriple_raise : public my_test_base<T>
{
public:
	numtriple_raise() : my_test_base<T>("numtriple_raise") { }

	void run()
	{
		NumTriple<T> x;
		x.RaiseError();
	}
};


template<typename T>
class valarray_vec : public my_test_base<T>
{
public:
	static const size_t N = 6;

	valarray_vec()
	: my_test_base<T>("valarray_compare"), x(N), y(N) { }

	std::valarray<T> x, y;

	void set_up()
	{
		for (size_t i = 0; i < 6; ++i)
		{
			x[i] = y[i] = T(i+1);
		}

		y[0] = T(10);
	}

	void run()
	{
		const T *vx = &x[0];
		const T *vy = &y[0];

		ASSERT_VEC_EQ(N, vx, vy);
	}
};




// organize test suites

AUTO_TPACK( numpair )
{
	ADD_TESTCASE( numpair_constructs<float> )
	ADD_TESTCASE( numpair_constructs<double> )
	ADD_TESTCASE( numpair_arith<float> )
	ADD_TESTCASE( numpair_arith<double> )
}

AUTO_TPACK( numtriple )
{
	ADD_TESTCASE( numtriple_constructs<float> )
	ADD_TESTCASE( numtriple_constructs<double> )
	ADD_TESTCASE( numtriple_arith<float> )
	ADD_TESTCASE( numtriple_arith<double> )
	ADD_TESTCASE( numtriple_raise<float> )
	ADD_TESTCASE( numtriple_raise<double> )
}

AUTO_TPACK( valarray )
{
	ADD_TESTCASE( valarray_vec<float> )
	ADD_TESTCASE( valarray_vec<double> )
}


int main(int argc, char *argv[])
{
	std_test_main(*auto_test_suite::main_suite());
}



