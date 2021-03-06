/**
 * @file test_assertions.h
 *
 *  A set of assertions for verifying testing condition
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_TEST_ASSERTIONS_H_
#define LIGHT_TEST_TEST_ASSERTIONS_H_

#include "base.h"
#include "float_accuracy.h"
#include <cmath>
#include <cstring>

namespace ltest
{
	class assertion_failure : public std::exception
	{
	public:
		assertion_failure(const char *file, unsigned int line, const char *assertion)
		: m_file(file)
		, m_line(line)
		, m_assertion(assertion)
		, m_message(std::string("Assertion failed: ") + m_assertion)
		{
		}

		virtual ~assertion_failure() throw() { }

		virtual const char *what() const throw() { return m_message.c_str(); }

	public:
		const char *file_name() const
		{
			return m_file.c_str();
		}

		unsigned int line_number() const
		{
			return m_line;
		}

		const char *assertion() const
		{
			return m_assertion.c_str();
		}

	private:
		std::string m_file;
		unsigned int m_line;
		std::string m_assertion;
		std::string m_message;

	}; // end class assertion_failure


	/************************************************
	 *
	 *  vector/matrix equality testing
	 *
	 ************************************************/

	template<typename TInt, class VecA, class VecB>
	inline bool test_vector_equal(TInt n, const VecA& a, const VecB& b)
	{
		for (TInt i = 0; i < n; ++i)
		{
			if (!(a[i] == b[i])) return false;
		}
		return true;
	}

    template<typename TInt, class VecA, typename T>
    inline bool test_vector_equals(TInt n, const VecA& a, const T& v)
    {
        for (TInt i = 0; i < n; ++i)
        {
            if (!(a[i] == v)) return false;
        }
        return true;
    }

	template<typename TInt, class VecA, class VecB, typename T>
	inline bool test_vector_approx(TInt n, const VecA& a, const VecB& b, T tol)
	{
		for (TInt i = 0; i < n; ++i)
		{
			if (!( std::fabs(a[i] - b[i]) <= tol )) return false;
		}
		return true;
	}

	template<typename TInt, class MatA, class MatB>
	inline bool test_matrix_equal(TInt m, TInt n, const MatA& a, const MatB& b)
	{
		for (TInt j = 0; j < n; ++j)
		{
			for (TInt i = 0; i < m; ++i)
			{
				if (!( a(i, j) == b(i, j) )) return false;
			}
		}
		return true;
	}

    template<typename TInt, class MatA, typename T>
    inline bool test_matrix_equals(TInt m, TInt n, const MatA& a, const T& v)
    {
        for (TInt j = 0; j < n; ++j)
        {
            for (TInt i = 0; i < m; ++i)
            {
                if (!( a(i, j) == v )) return false;
            }
        }
        return true;
    }

	template<typename TInt, class MatA, class MatB, typename T>
	inline bool test_matrix_approx(TInt m, TInt n, const MatA& a, const MatB& b, T tol)
	{
		for (TInt j = 0; j < n; ++j)
		{
			for (TInt i = 0; i < m; ++i)
			{
				if (!( std::fabs(a(i,j) - b(i,j)) <= tol )) return false;
			}
		}
		return true;
	}

}

/************************************************
 *
 *  Macros
 *
 ************************************************/

#define ASSERT_TRUE( cond ) \
	if (!(cond)) throw ::ltest::assertion_failure(__FILE__, __LINE__, #cond " is TRUE")

#define ASSERT_FALSE( cond ) \
	if ((cond)) throw ::ltest::assertion_failure(__FILE__, __LINE__, #cond " is FALSE")

#define ASSERT_EQ( a, b ) \
	if (!((a) == (b))) throw ::ltest::assertion_failure(__FILE__, __LINE__, #a " == " #b)

#define ASSERT_NE( a, b ) \
	if (!((a) != (b))) throw ::ltest::assertion_failure(__FILE__, __LINE__, #a " != " #b)

#define ASSERT_APPROX( a, b, tol ) \
	if (!( std::fabs((a) - (b)) <= tol )) throw ::ltest::assertion_failure(__FILE__, __LINE__, #a " ~= " #b)

#define ASSERT_ULP( a, b, dtol ) \
	if ( ::ltest::ulp_distance(a, b) > dtol ) throw ::ltest::assertion_failure(__FILE__, __LINE__, "ULP(" #a ", " #b ") <= " #dtol)

#define ASSERT_CT_VALUE( T, V ) \
	if (!((T::value) == (V))) throw ::ltest::assertion_failure(__FILE__, __LINE__, #T "::value == " #V)

#define ASSERT_SAME_TYPE( T, R ) \
	if (!std::is_same<T, R>::value) throw ::ltest::assertion_failure(__FILE__, __LINE__, #T " is " #R)

#define ASSERT_STREQ( a, b ) \
	if (!(std::string(a) == std::string(b))) throw ::ltest::assertion_failure(__FILE__, __LINE__, #a " == " #b)

#define ASSERT_VEC_EQ( n, a, b ) \
	if (!::ltest::test_vector_equal(n, a, b)) \
		throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #n "] == " #b "[0:" #n "]" )

#define ASSERT_VEC_EQS( n, a, v ) \
    if (!::ltest::test_vector_equals(n, a, v)) \
        throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #n "] == " #v )

#define ASSERT_MAT_EQ( m, n, a, b ) \
	if (!::ltest::test_matrix_equal(m, n, a, b)) \
		throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #m ", 0:" #n "] == " #b "[0:" #m ", 0:" #n "]")

#define ASSERT_MAT_EQS( m, n, a, v ) \
    if (!::ltest::test_matrix_equals(m, n, v)) \
        throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #m ", 0:" #n "] == " #v )

#define ASSERT_VEC_APPROX( n, a, b, tol ) \
	if (!::ltest::test_vector_approx(n, a, b, tol)) \
		throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #n "] ~= " #b "[0:" #n "]" )

#define ASSERT_MAT_APPROX( m, n, a, b, tol ) \
	if (!::ltest::test_matrix_approx(m, n, a, b, tol)) \
		throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #m ", 0:" #n "] ~= " #b "[0:" #m ", 0:" #n "]")


#endif /* TEST_ASSERTIONS_H_ */




