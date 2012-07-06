/**
 * @file float_accuracy.h
 *
 * The functions to measure floating-point evaluation accuracy
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_FLOAT_ACCURACY_H_
#define LIGHT_TEST_FLOAT_ACCURACY_H_

#include "base.h"

#ifdef LTEST_USE_C11_STDLIB
#include <cstdint>
#else
#include <stdint.h>
#endif

#include <stdexcept>
#include <cmath>

namespace ltest
{
#ifdef LTEST_USE_C11_STDLIB
	using std::uint32_t;
	using std::uint64_t;
#else
	using ::uint32_t;
	using ::uint64_t;
#endif

	namespace detail
	{
		template<typename T> struct float_format;

		template<>
		struct float_format<float>
		{
			typedef uint32_t repr_int_type;

			static const int exponent_bits = 8;
			static const int mantissa_bits = 23;

			static const unsigned int special_exponent = 255;

			static const repr_int_type mantissa_mask   = 0x007fffffU;
			static const repr_int_type hidden_bit_mask = 0x00800000U;
		};


		template<> struct float_format<double>
		{
			typedef uint64_t repr_int_type;

			static const int exponent_bits = 11;
			static const int mantissa_bits = 52;

			static const unsigned int special_exponent = 2047;

			static const repr_int_type mantissa_mask   = 0x000fffffffffffffULL;
			static const repr_int_type hidden_bit_mask = 0x0010000000000000ULL;
		};


		template<typename T>
		inline typename float_format<T>::repr_int_type
		extract_mantissa(T x, unsigned int& e)  // x > 0
		{
			typedef float_format<T> fmt;

			union {
				T v;
				typename fmt::repr_int_type u;
			} c;

			c.v = x;

			e = (unsigned int)(c.u >> fmt::mantissa_bits);
			return c.u & fmt::mantissa_mask;
		}


		template<typename TInt>
		inline unsigned int ulp_int(TInt a)
		{
			unsigned int d = 0;
			while (a)
			{
				a >>= 1;
				++d;
			}

			return d;
		}

		template<typename T>
		inline unsigned int ulp_distance_p(const T& a, const T& b)  // apply when 0 <= a < b < inf
		{
			typedef float_format<T> fmt;
			typedef typename fmt::repr_int_type bitp_t;

			unsigned int ea, eb;

			bitp_t ma = extract_mantissa(a, ea);
			bitp_t mb = extract_mantissa(b, eb);

			if (ea > 0 && eb > 0)
			{
				ma |= fmt::hidden_bit_mask;
				mb |= fmt::hidden_bit_mask;

				mb <<= (eb - ea);
				return ulp_int(mb - ma);
			}
			else if (ea == 0 && eb == 0)
			{
				return ulp_int(mb - ma);
			}
			else
			{
				return fmt::mantissa_bits + 1;
			}
		}

	}

	template <typename T>
	inline unsigned int ulp_distance(T a, T b)
	{
	   	if(!std::isfinite(a))
			throw std::invalid_argument("a is NOT finite.");

		if (!std::isfinite(b))
			throw std::invalid_argument("b is NOT finite.");

		if (a == b)
			return 0;

		if (a > b) // swap
		{
			T tmp = a;
			a = b;
			b = tmp;
		}

		// at this point, a < b guaranteed

		if (a < T(0))
		{
			if (b <= T(0))
				return detail::ulp_distance_p(-b, -a);
			else if (b == T(0))
				return detail::ulp_distance_p(T(0), -a);
			else
			{
				unsigned int da = detail::ulp_distance_p(T(0), -a);
				unsigned int db = detail::ulp_distance_p(T(0), b);
				return (da > db ? da : db) + 1;
			}
		}
		else
		{
			return detail::ulp_distance_p(a, b);
		}
	}

}

#endif
