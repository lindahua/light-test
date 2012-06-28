/**
 * @file test_mon.h
 *
 * The test monitoring class
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#pragma warning(push)
#pragma warning(disable : 4100)
#endif

#ifndef LIGHT_TEST_TEST_MON_H_
#define LIGHT_TEST_TEST_MON_H_

#include "base.h"

namespace ltest
{
	class test_monitor
	{
	public:
		virtual ~test_monitor() { }

		virtual void on_suite_begin(const test_suite& tsuite) { }

		virtual void on_suite_end(const test_suite& tsuite, size_t npassed_cases) { }

		virtual void on_pack_begin(const test_pack& tpack) { }

		virtual void on_pack_end(const test_pack& tpack, size_t npassed_cases) { }

		virtual void on_case_begin(const test_case& tcase) { }

		virtual void on_case_end(const test_case& tcase, bool is_passed) { }

		virtual void on_assertion_failure(const assertion_failure& e) { }

		virtual void on_exception(const std::exception& e) { }
	};

}

#ifdef _MSC_VER
#pragma warning(pop)
#endif


#endif /* TEST_MON_H_ */
