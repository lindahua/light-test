/**
 * @file timer.h
 *
 * @brief High quality timer for benchmark
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_TIMER_H_
#define LIGHT_TEST_TIMER_H_

#include "internal/timer_impl.h"

namespace ltest
{
	class timer
	{
		typedef typename internal::timer_impl::time_type time_type;
	public:
		LTEST_ENSURE_INLINE
		explicit timer( bool to_start = false )
		{
			if (to_start) start();
		}

		LTEST_ENSURE_INLINE
		void start()
		{
			m_impl.get_current_time(m_start_t);
		}

		LTEST_ENSURE_INLINE
		double elapsed_secs() const
		{
			return elapsed_nsecs() * 1.0e-9;
		}

		LTEST_ENSURE_INLINE
		double elapsed_msecs() const
		{
			return elapsed_nsecs() * 1.0e-6;
		}

		LTEST_ENSURE_INLINE
		double elapsed_usecs() const
		{
			return elapsed_nsecs() * 1.0e-3;
		}

		LTEST_ENSURE_INLINE
		double elapsed_nsecs() const
		{
			time_type t;
			m_impl.get_current_time(t);
			return m_impl.calc_time_distance(m_start_t, t);
		}

	private:
		time_type m_start_t;
		internal::timer_impl m_impl;
	};


}

#endif /* TIMER_H_ */
