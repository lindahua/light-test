/**
 * @file example2.cpp
 *
 * @brief A simple example to demonstrate benchmarking
 *
 * @author Dahua Lin
 */

#include "../light_test/benchmark.h"
#include "../light_test/std_bench_mon.h"

#include <cmath>

using namespace ltest;

struct bench_math_base
{
	const char *_name;
	size_t _size;
	const double *_src;
	double *_dst;

	bench_math_base(const char* name, size_t siz, const double* src, double *dst)
	: _name(name), _size(siz), _src(src), _dst(dst) { }

	const char* name() const
	{
		return _name;
	}

	size_t size() const
	{
		return _size;
	}
};



struct bench_sqrt : bench_math_base
{
	bench_sqrt(size_t n, const double* src, double *dst)
	: bench_math_base("sqrt", n, src, dst) { }

	void operator() () const
	{
		for (size_t i = 0; i < _size; ++i) _dst[i] = std::sqrt(_src[i]);
	}
};

struct bench_exp : bench_math_base
{
	bench_exp(size_t n, const double* src, double *dst)
	: bench_math_base("exp", n, src, dst) { }

	void operator() () const
	{
		for (size_t i = 0; i < _size; ++i) _dst[i] = std::exp(_src[i]);
	}
};

struct bench_log : bench_math_base
{
	bench_log(size_t n, const double* src, double *dst)
	: bench_math_base("log", n, src, dst) { }

	void operator() () const
	{
		for (size_t i = 0; i < _size; ++i) _dst[i] = std::log(_src[i]);
	}
};

struct bench_sin : bench_math_base
{
	bench_sin(size_t n, const double* src, double *dst)
	: bench_math_base("sin", n, src, dst) { }

	void operator() () const
	{
		for (size_t i = 0; i < _size; ++i) _dst[i] = std::sin(_src[i]);
	}
};




int main(int argc, char *argv[])
{
	const size_t N = 1000;
	const char *templ_spec = "{{jobname : %-5s}}:  {{times: %10lu}}  | {{secs: %10.4f}} s  | {{mps: %10.2f}} MPS\n";
	std_bench_monitor mon(templ_spec);
	benchmark_option opt(2000);

	double *src = new double[N];
	double *dst = new double[N];

	std::cout << "benchmark results:\n";
	run_benchmark(bench_sqrt(N, src, dst), mon, opt);
	run_benchmark(bench_exp (N, src, dst), mon, opt);
	run_benchmark(bench_log (N, src, dst), mon, opt);
	run_benchmark(bench_sin (N, src, dst), mon, opt);

	delete [] src;
	delete [] dst;
}

