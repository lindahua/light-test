/**
 * @file std_bench_mon.h
 *
 * @brief Standard benchmarking monitor
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_STD_BENCH_MON_H_
#define LIGHT_TEST_STD_BENCH_MON_H_

#include "str_template.h"
#include "timer.h"

#define LTEST_STD_REPORT_TEMPLATE "{{jobname : %-28s}}:  {{times: %10lu}}  | {{secs: %10.4f}} s  | {{mps: %10.2f}} MPS\n"

namespace ltest
{

	class bench_report_source
	{
	public:
		template<class Job>
		bench_report_source(const Job& job, size_t n, const runtime_span& span)
		: m_jobname(job.name())
		, m_jobsize(job.size())
		, m_times(n)
		, m_runsize(m_jobsize * m_times)
		, m_span(span)
		{ }

		std::string operator() (const char *name, const char *fmt=0) const
		{
			if (str_eq(name, "jobname")) return _fmt(m_jobname, fmt);
			else if (str_eq(name, "jobsize")) return _fmt(m_jobsize, fmt);
			else if (str_eq(name, "times")) return _fmt(m_times, fmt);
			else if (str_eq(name, "secs"))  return _fmt(m_span.secs(), fmt);
			else if (str_eq(name, "msecs")) return _fmt(m_span.msecs(), fmt);
			else if (str_eq(name, "usecs")) return _fmt(m_span.usecs(), fmt);
			else if (str_eq(name, "nsecs")) return _fmt(m_span.nsecs(), fmt);
			else if (str_eq(name, "gps")) return _fmt(m_span.gps(m_runsize), fmt);
			else if (str_eq(name, "mps")) return _fmt(m_span.mps(m_runsize), fmt);
			else if (str_eq(name, "kps")) return _fmt(m_span.kps(m_runsize), fmt);
			else if (str_eq(name, "ps"))  return _fmt(m_span.ps(m_runsize), fmt);
			else return "####";
		}

	private:
		static std::string _fmt(const std::string& v, const char *fmt)
		{
			return fmt ? sformat(v.c_str(), fmt) : v;
		}

		static std::string _fmt(double v, const char *fmt)
		{
			return sformat(v, fmt ? fmt : "%g");
		}

		static std::string _fmt(size_t v, const char *fmt)
		{
			return sformat(v, fmt ? fmt : "%lu");
		}

	private:
		std::string m_jobname;
		size_t m_jobsize;
		size_t m_times;
		size_t m_runsize;
		runtime_span m_span;
	};


	class std_bench_monitor
	{
	public:
		explicit std_bench_monitor()
		: m_report_template(LTEST_STD_REPORT_TEMPLATE)
		, m_channel(std::cout) { _init(); }

		explicit std_bench_monitor(std::ostream& out)
		: m_report_template(LTEST_STD_REPORT_TEMPLATE)
		, m_channel(out) { _init(); }

		explicit std_bench_monitor(const char *templ)
		: m_report_template(templ)
		, m_channel(std::cout) { _init(); }

		std_bench_monitor(const char *templ, std::ostream& out)
		: m_report_template(templ)
		, m_channel(out) { _init(); }

		template<class Job>
		void operator() (const Job& job, size_t n, const runtime_span& span)
		{
			bench_report_source src(job, n, span);
			m_channel << src;
		}

	private:
		void _init()
		{
			m_channel << m_report_template;
		}

	private:
		str_template m_report_template;
		template_channel m_channel;
	};

}

#endif
