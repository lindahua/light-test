/**
 * @file auto_suite.h
 *
 * Test suite with auto registration
 * 
 * @author Dahua Lin 
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_AUTO_SUITE_H_
#define LIGHT_TEST_AUTO_SUITE_H_

#include "test_units.h"
#include <cstdio>

namespace ltest
{

	extern test_suite& auto_main_suite();

#ifdef LTEST_MAINSUITE_NAME
	test_suite& auto_main_suite()
	{
		static shared_ptr<test_suite> psuite;

		if (!psuite)
		{
			psuite.reset(new test_suite(LTEST_MAINSUITE_NAME));
		}

		return *psuite;
	}
#endif

	inline void register_testpack(test_pack* tpack)
	{
		auto_main_suite().add(tpack);
	}


	class auto_test_pack
	{
	public:
		auto_test_pack(const char* name)
		{
			m_ppack = new test_pack(name);
			register_testpack(m_ppack);
		}

		void add(test_case* pcase)
		{
			m_ppack->add(pcase);
		}

	private:
		auto_test_pack(const auto_test_pack& );
		auto_test_pack& operator = (const auto_test_pack& );

	protected:
		test_pack* m_ppack;
	};

}

// Useful macros


#define AUTO_TPACK( PackName ) \
	class ltest_pack_##PackName : public ltest::auto_test_pack { \
	public: \
		ltest_pack_##PackName(); }; \
	ltest_pack_##PackName ltest_pack_##PackName##_instance; \
	ltest_pack_##PackName::ltest_pack_##PackName() : ltest::auto_test_pack( #PackName )


#define ADD_TESTCASE( ClassName ) this->add(new ClassName());


#endif 
