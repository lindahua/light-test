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
	class runtime_span
	{
	public:
		LTEST_ENSURE_INLINE
		static runtime_span from_nsecs(double ns)
		{
			return runtime_span(ns);
		}

		LTEST_ENSURE_INLINE
		static runtime_span from_usecs(double us)
		{
			return runtime_span(us * 1.0e3);
		}

		LTEST_ENSURE_INLINE
		static runtime_span from_msecs(double ms)
		{
			return runtime_span(ms * 1.0e6);
		}

		LTEST_ENSURE_INLINE
		static runtime_span from_secs(double s)
		{
			return runtime_span(s * 1.0e9);
		}

	public:

		LTEST_ENSURE_INLINE
		double secs() const { return m_ns * 1.0e-9; }

		LTEST_ENSURE_INLINE
		double msecs() const { return m_ns * 1.0e-6; }

		LTEST_ENSURE_INLINE
		double usecs() const { return m_ns * 1.0e-3; }

		LTEST_ENSURE_INLINE
		double nsecs() const { return m_ns; }

		LTEST_ENSURE_INLINE
		double ps(size_t n) const { return double(n) * 1.0e9 / m_ns; }

		LTEST_ENSURE_INLINE
		double kps(size_t n) const { return double(n) * 1.0e6 / m_ns; }

		LTEST_ENSURE_INLINE
		double mps(size_t n) const { return double(n) * 1.0e3 / m_ns; }

		LTEST_ENSURE_INLINE
		double gps(size_t n) const { return double(n) / m_ns; }

	private:
		LTEST_ENSURE_INLINE
		runtime_span(double ns) : m_ns(ns) { }

		double m_ns;
	};

	class timer
	{
		typedef internal::timer_impl::time_type time_type;
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
		runtime_span elapsed() const
		{
			return runtime_span::from_nsecs(elapsed_nsecs());
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
