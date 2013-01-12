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

#ifndef LTEST_MAINSUITE_NAME
#define LTEST_MAINSUITE_NAME "Main"
#endif

namespace ltest
{

	class auto_test_suite
	{
	public:
		static test_suite* main_suite()
		{
			if (!m_psuite)
			{
				m_psuite.reset(new test_suite(LTEST_MAINSUITE_NAME));
			}

			return m_psuite.get();
		}

		static void register_testpack(test_pack* tpack)
		{
			main_suite()->add(tpack);
		}

	private:
		static shared_ptr<test_suite> m_psuite;
	};


	class auto_test_pack
	{
	public:
		auto_test_pack(const char* name)
		{
			m_ppack = new test_pack(name);
			auto_test_suite::register_testpack(m_ppack);
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



#define LTEST_INIT_AUTOSUITE extern ltest::shared_ptr<ltest::test_suite> ltest::auto_test_suite::m_psuite;

#define AUTO_TPACK( PackName ) \
	class ltest_pack_##PackName : public ltest::auto_test_pack { \
	public: \
		ltest_pack_##PackName(); }; \
	ltest_pack_##PackName ltest_pack_##PackName##_instance; \
	ltest_pack_##PackName::ltest_pack_##PackName() : ltest::auto_test_pack( #PackName )


#define ADD_TESTCASE( ClassName ) this->add(new ClassName());


#endif 
