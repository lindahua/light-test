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

namespace ltest
{

#define _LTEST_DEFINE_OPTION_FIELD(T, name) \
		T name; \
		benchmark_option& set_##name(T v) { \
			name = v; \
			return *this; }

	struct benchmark_option
	{
		_LTEST_DEFINE_OPTION_FIELD( size_t, batch_size )
		_LTEST_DEFINE_OPTION_FIELD( size_t, max_nbatches )
		_LTEST_DEFINE_OPTION_FIELD( size_t, warming_runs )
		_LTEST_DEFINE_OPTION_FIELD( double, time_thres )

		explicit benchmark_option(size_t bsize)
		: batch_size(bsize)
		, max_nbatches(1000000000)
		, warming_runs(5)
		, time_thres(0.5)
		{ }
	};


	template<class Job, class Monitor>
	inline void run_benchmark(const Job& job, Monitor& mon,
			const benchmark_option& option)
	{
		for (size_t i = 0; i < option.warming_runs; ++i)
			job();

		const size_t maxn = option.max_nbatches;
		const size_t bsiz = option.batch_size;
		const double tthres = option.time_thres;

		size_t k = 0;

		timer tm(true);
		while (tm.elapsed_secs() < tthres && k < maxn)
		{
			for (size_t i = 0; i < bsiz; ++i)
				job();

			++k;
		}

		mon(job, k * bsiz, tm.elapsed());
	}

}

#endif
