/**
 * @file timer_impl.h
 *
 * @brief Timer implementation
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_TIMER_IMPL_H_
#define LIGHT_TEST_TIMER_IMPL_H_

#include <time.h>
#include <sys/time.h>

#ifdef __MACH__
#include <mach/mach_time.h>
#endif

#if (defined(_WIN32) || defined(_WIN64)) && defined(_MSC_VER)
	#define LTEST_ENSURE_INLINE __forceinline
#else
	#define LTEST_ENSURE_INLINE __attribute__((always_inline))
#endif

namespace ltest { namespace internal {

#ifdef __MACH__

	class timer_impl
	{
	public:
		typedef uint64_t time_type;

		timer_impl()
		{
			::mach_timebase_info(&m_baseinfo);
		}

		LTEST_ENSURE_INLINE
		void get_current_time(time_type& t) const
		{
			t = ::mach_absolute_time();
		}

		LTEST_ENSURE_INLINE
		double calc_time_distance(const time_type& t0, const time_type& t1) const
		{
			uint64_t d = (m_baseinfo.numer * (t1 - t0)) / m_baseinfo.denom;
			return static_cast<double>(d);
		}

	private:
		mach_timebase_info_data_t m_baseinfo;
	};

#else

	class timer_impl
	{
	public:
		typedef timespec time_type;

		LTEST_ENSURE_INLINE
		void get_current_time(time_type& t) const
		{
			::clock_gettime(CLOCK_REALTIME, &t);
		}

		LTEST_ENSURE_INLINE
		double calc_time_distance(const time_type& t0, const time_type& t1) const
		{
			return double(t1.tv_sec - t0.tv_sec) * 1.0e9 +
					double(t1.tv_nsec - t0.tv_nsec);
		}
	};

#endif


} }

#endif
