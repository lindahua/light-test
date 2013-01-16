/**
 * @file benchmark.h
 *
 * @brief Support of benchmark
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_BENCHMARK_H_
#define LIGHT_TEST_BENCHMARK_H_

#include "base.h"
#include "timer.h"
#include <iostream>
#include <cmath>

namespace ltest
{

#define _LTEST_DEFINE_OPTION_FIELD(T, name) \
		T name; \
		benchmark_option& set_##name(T v) { \
			name = v; \
			return *this; }

	struct benchmark_option
	{
		_LTEST_DEFINE_OPTION_FIELD( size_t, probe_batch_size )
		_LTEST_DEFINE_OPTION_FIELD( size_t, max_batch_size )
		_LTEST_DEFINE_OPTION_FIELD( size_t, warming_runs )
		_LTEST_DEFINE_OPTION_FIELD( double, time_thres )
		_LTEST_DEFINE_OPTION_FIELD( double, batch_ratio )

		explicit benchmark_option(size_t bsize0)
		: probe_batch_size(bsize0)
		, max_batch_size(2000000000)
		, warming_runs(5)
		, time_thres(0.5)
		, batch_ratio(0.1)
		{ }
	};


	inline size_t determine_bench_batch_size(const benchmark_option& option, double probe_time)
	{
		double bt = option.batch_ratio * option.time_thres;

		size_t bsiz = option.probe_batch_size;

		if (probe_time < bt)
		{
			size_t m = (size_t)std::ceil(bt / probe_time);
			size_t max_m = option.max_batch_size / option.probe_batch_size;
			if (m > max_m) m = max_m;

			if (m > 1)
			{
				bsiz *= m;
				if (bsiz > 100)
				{
					m = 1;
					while (bsiz >= 100)
					{
						bsiz /= 10;
						m *= 10;
					}

					bsiz *= m;
				}
			}
		}

		return bsiz;
	}


	template<class Job, class Monitor>
	inline void run_benchmark(const Job& job, Monitor& mon,
			const benchmark_option& option)
	{
		// warming

		for (size_t i = 0; i < option.warming_runs; ++i) job();

		// probing

		size_t bsiz = option.probe_batch_size;
		timer tm0(true);
		for (size_t i = 0; i < bsiz; ++i) job();
		double pt = tm0.elapsed_secs();

		// determine batch size

		bsiz = determine_bench_batch_size(option, pt);

		timer tm;
		double et = 0.0;
		double tt = option.time_thres;
		size_t k = 0;

		while (et < tt)
		{
			tm.start();
			for (size_t i = 0; i < bsiz; ++i) job();
			et += tm.elapsed_secs();
			++k;
		}

		mon(job, k * bsiz, runtime_span::from_secs(et));
	}

}

#endif
