/**
 * @file test_exec.h
 *
 * Test execution under monitoring
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_TEST_EXEC_H_
#define LIGHT_TEST_TEST_EXEC_H_

#include "test_assertions.h"
#include "test_units.h"
#include "test_mon.h"

namespace ltest
{

	inline bool execute_case(test_case& tcase, test_monitor& mon)
	{
		bool passed = true;
		mon.on_case_begin(tcase);

		try
		{
			tcase.set_up();
			try
			{
				tcase.run();
			}
			catch( assertion_failure& e)
			{
				passed = false;
				mon.on_assertion_failure(e);
			}

			tcase.tear_down();
		}
		catch( std::exception& e)
		{
			passed = false;
			mon.on_exception(e);
		}

		mon.on_case_end(tcase, passed);
		return passed;
	}


	inline size_t execute_pack(test_pack& tpack, test_monitor& mon)
	{
		size_t n = tpack.size();
		size_t npassed = 0;

		mon.on_pack_begin(tpack);

		for (size_t i = 0; i < n; ++i)
		{
			if (execute_case(tpack.tcase(i), mon)) ++npassed;
		}

		mon.on_pack_end(tpack, npassed);
		return npassed;
	}


	inline size_t execute_suite(test_suite& tsuite, test_monitor& mon)
	{
		size_t npassed = 0;

		mon.on_suite_begin(tsuite);

		for (size_t i = 0; i < tsuite.size(); ++i)
		{
			npassed += execute_pack(tsuite.tpack(i), mon);
		}

		mon.on_suite_end(tsuite, npassed);
		return npassed;
	}

}

#endif /* TEST_EXEC_H_ */
