/**
 * @file test_units.h
 *
 * The test unit classes (test_case and test_suite)
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_TEST_UNITS_H_
#define LIGHT_TEST_TEST_UNITS_H_

#include "base.h"
#include <vector>

namespace ltest
{

	class test_case
	{
	public:
		virtual ~test_case() { };

		virtual const char *name() const = 0;

		virtual void run() = 0;

		virtual void set_up() { }

		virtual void tear_down() { }

	}; // end class test_case



	class test_pack
	{
	public:
		test_pack( const char *name )
		: m_name(name)
		{
		}

		test_pack( const std::string& name )
		: m_name(name)
		{
		}

		virtual ~test_pack() { }

	public:
		const char *name() const
		{
			return m_name.c_str();
		}

		void add(test_case *pcase)
		{
			m_cases.push_back(shared_ptr<test_case>(pcase));
		}

		size_t size() const
		{
			return m_cases.size();
		}

		const test_case& tcase(size_t i) const
		{
			return *(m_cases[i]);
		}

		test_case& tcase(size_t i)
		{
			return *(m_cases[i]);
		}

	private:
		std::string m_name;
		std::vector<shared_ptr<test_case> > m_cases;

	}; // end class test_pack


	class test_suite
	{
	public:
		test_suite( const char *name )
		: m_name(name)
		{
		}

		test_suite( const std::string& name )
		: m_name(name)
		{
		}

	public:
		const char *name() const
		{
			return m_name.c_str();
		}

		void add(test_pack *ppack)
		{
			m_packs.push_back(shared_ptr<test_pack>(ppack));
		}

		size_t size() const
		{
			return m_packs.size();
		}

		const test_pack& tpack(size_t i) const
		{
			return *(m_packs[i]);
		}

		test_pack& tpack(size_t i)
		{
			return *(m_packs[i]);
		}

	private:
		std::string m_name;
		std::vector<shared_ptr<test_pack> > m_packs;

	}; // end class test_suite

}

#endif /* TEST_UNITS_H_ */
