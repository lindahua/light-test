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

	template<typename TInt, class VecA, class VecB, typename T>
	inline bool test_vector_approx(TInt n, const VecA& a, const VecB& b, T tol)
	{
		for (TInt i = 0; i < n; ++i)
		{
			if ( std::fabs(a[i] - b[i]) > tol ) return false;
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

	template<typename TInt, class MatA, class MatB, typename T>
	inline bool test_matrix_approx(TInt m, TInt n, const MatA& a, const MatB& b, T tol)
	{
		for (TInt j = 0; j < n; ++j)
		{
			for (TInt i = 0; i < m; ++i)
			{
				if ( std::fabs(a(i,j) - b(i,j)) > tol ) return false;
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

#define ASSERT_VEC_EQ( n, a, b ) \
	if (!::ltest::test_vector_equal(n, a, b)) \
		throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #n "] == " #b "[0:" #n "]" )

#define ASSERT_MAT_EQ( m, n, a, b ) \
	if (!::ltest::test_matrix_equal(m, n, a, b)) \
		throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #m ", 0:" #n "] == " #b "[0:" #m ", 0:" #n "]")

#define ASSERT_VEC_APPROX( n, a, b, tol ) \
	if (!::ltest::test_vector_approx(n, a, b, tol)) \
		throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #n "] ~= " #b "[0:" #n "]" )

#define ASSERT_MAT_APPROX( m, n, a, b, tol ) \
	if (!::ltest::test_matrix_approx(m, n, a, b, tol)) \
		throw ::ltest::assertion_failure(__FILE__, __LINE__, #a "[0:" #m ", 0:" #n "] ~= " #b "[0:" #m ", 0:" #n "]")


#endif /* TEST_ASSERTIONS_H_ */




