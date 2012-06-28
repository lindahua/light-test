/**
 * @file base.h
 *
 * The basic definitions for the light-test library
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_BASE_H_
#define LIGHT_TEST_BASE_H_

#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
	#if _MSC_VER < 1600
		#error Microsoft Visual C++ of version lower than MSVC 2010 is not supported.
	#endif
	#define LTEST_USE_C11_STDLIB

#elif (defined(__GNUC__))

	#if (defined(__clang__))
		#if ((__clang_major__ < 2) || (__clang_major__ == 2 && __clang_minor__ < 8))
			#error CLANG of version lower than 2.8.0 is not supported
		#endif
	#else
		#if ((__GNUC__ < 4) || (__GNUC__ == 4 && __GNUC_MINOR__ < 2))
			#error GCC of version lower than 4.2.0 is not supported
		#endif
	#endif

	#if (defined(__GXX_EXPERIMENTAL_CXX0X__))
		#define LTEST_USE_C11_STDLIB
	#endif

#else
	#error Light-Test can only be used with Microsoft Visual C++, GCC (G++), or clang (clang++).
#endif

#include <cstddef>
#include <string>
#include <exception>
#include <iosfwd>

#if defined(LTEST_USE_C11_STDLIB)
#include <memory>
#else
#include <tr1/memory>
#endif


namespace ltest
{
	using std::size_t;

#if defined(LTEST_USE_C11_STDLIB)
	using std::shared_ptr;
#else
	using std::tr1::shared_ptr;
#endif

	class assertion_failure;

	class test_case;
	class test_pack;
	class test_suite;
	class test_monitor;
}

#endif /* BASE_H_ */









