/**
 * @file std_test_mon.h
 *
 * Standard testing monitor
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_STD_TEST_MON_H_
#define LIGHT_TEST_STD_TEST_MON_H_

#include "test_assertions.h"
#include "test_units.h"
#include "test_mon.h"
#include "test_exec.h"

#include "color_printf.h"

namespace ltest
{
	class std_test_monitor : public test_monitor
	{
	public:
		std_test_monitor()
		{
			m_isuite = 0;
			m_ipack = 0;
			m_icase = 0;

			m_finished_cases = 0;
			m_passed_cases = 0;
		}

		size_t total_finished_cases() const
		{
			return m_finished_cases;
		}

		size_t total_passed_cases() const
		{
			return m_passed_cases;
		}

		bool is_all_passed() const
		{
			return m_finished_cases == m_passed_cases;
		}


	public:
		virtual void on_suite_begin(const test_suite& tsuite)
		{
			++ m_isuite;
			m_csuite = &tsuite;
			print_suite_begin(tsuite);
		}

		virtual void on_suite_end(const test_suite& tsuite, size_t npassed_cases)
		{
			print_suite_end(tsuite, npassed_cases);

			m_finished_cases += tsuite.num_cases();
			m_passed_cases += npassed_cases;

			m_ipack = 0;
			m_icase = 0;
		}

		virtual void on_pack_begin(const test_pack& tpack)
		{
			++ m_ipack;
			m_cpack = &tpack;
			print_pack_begin(tpack);
		}

		virtual void on_pack_end(const test_pack& tpack, size_t npassed_cases)
		{
			print_pack_end(tpack, npassed_cases);

			m_icase = 0;
		}

		virtual void on_case_begin(const test_case& tcase)
		{
			++ m_icase;
			print_case_begin(tcase);
		}

		virtual void on_case_end(const test_case& tcase, bool is_passed)
		{
			print_case_end(tcase, is_passed);
		}

		virtual void on_assertion_failure(const assertion_failure& e)
		{
			print_assertion_failure(e);
		}

		virtual void on_exception(const std::exception& e)
		{
			print_exception(e);
		}

	private:
		void print_suite_begin(const test_suite& tsuite)
		{
			printf_with_color(color_unittype, "Test Suite ");
			printf_with_color_bold(color_unitname, "%s\n", tsuite.name());
			printf_with_color(color_sepline, "=============================================\n");
		}

		void print_suite_end(const test_suite& tsuite, size_t npassed_cases)
		{
			printf_with_color(color_sepline, "=============================================\n");
			printf_with_color(color_unittype, "Test Suite ");
			printf_with_color_bold(color_unitname, "%s ", tsuite.name());
			printf_with_color_bold(color_stats, "%lu / %lu cases passed\n",
					npassed_cases, tsuite.num_cases());

			std::printf("\n");
		}

		void print_pack_begin(const test_pack& tpack)
		{
			if (m_csuite)
			{
				printf_with_color(color_unittype, "Test Pack ");
				printf_with_color(color_unittype, "[%3lu / %3lu]: ", m_ipack, m_csuite->size());
			}
			else
			{
				printf_with_color(color_unittype, "Test Pack: ");
			}

			printf_with_color_bold(color_unitname, "%s", tpack.name());
			printf_with_color(color_sepline, "\n----------------------\n");
		}

		void print_pack_end(const test_pack& tpack, size_t npassed_cases)
		{
			printf_with_color(color_sepline, "----------------------\n");
			printf_with_color_bold(color_stats, "%lu / %lu cases passed\n", npassed_cases, tpack.size());
			std::printf("\n");
		}

		void print_case_begin(const test_case& tcase)
		{
			if (m_cpack)
			{
				std::string title = std::string(m_cpack->name()) + "." + tcase.name();

				printf_with_color(color_unittype, "  [%3lu / %3lu]: ",
						m_icase, m_cpack->size());

				printf_with_color(color_unitname, " %-40s", title.c_str());
			}
			else
			{
				printf_with_color(color_unittype, "  [%3lu]: ", m_icase);
				printf_with_color(color_unitname, " %-28s", tcase.name());
			}

			printf_with_color(color_unittype, "  ...  ");
		}

		void print_case_end(const test_case&, bool passed)
		{
			if (passed)
			{
				printf_with_color(color_pass, "passed\n");
			}
		}

		void print_assertion_failure(const assertion_failure& e)
		{
			printf_with_color(color_fail, "failed\n");
			printf_with_color_bold(color_location, "   **** %s (%u): ", e.file_name(), e.line_number());
			std::printf("%s\n", e.what());
			std::printf("\n");
		}

		void print_exception(const std::exception& e)
		{
			const char *msg = e.what() != 0 ? e.what() : "Unknown cause";

			printf_with_color(color_fail, "failed\n");
			std::printf("   **** STD Exception: %s\n", msg);
			std::printf("\n");
		}

	public:
		static const color_t color_fail = LTCOLOR_RED;
		static const color_t color_pass = LTCOLOR_GREEN;
		static const color_t color_unittype = LTCOLOR_BROWN;
		static const color_t color_unitname = LTCOLOR_BROWN;
		static const color_t color_stats = LTCOLOR_GREEN;
		static const color_t color_location = LTCOLOR_CYAN;
		static const color_t color_sepline = LTCOLOR_BLUE;

	private:
		const test_suite * m_csuite;
		const test_pack  * m_cpack;

		size_t m_isuite;
		size_t m_ipack;
		size_t m_icase;

		size_t m_finished_cases; // update upon the end of a suite
		size_t m_passed_cases;   // update upon the end of a suite

	}; // end class std_test_monitor



	inline bool std_test_main(test_suite& master_suite)
	{
		std_test_monitor mon;
		execute_suite(master_suite, mon);

		bool all_passed = mon.is_all_passed();

		if (all_passed)
		{
			printf_with_color_bold(std_test_monitor::color_pass,
					"All %lu cases passed!\n", mon.total_finished_cases());
		}
		else
		{
			printf_with_color_bold(std_test_monitor::color_fail,
					"Total %lu / %lu cases failed!\n",
					mon.total_finished_cases() - mon.total_passed_cases(),
					mon.total_finished_cases());
		}

		std::printf("\n\n");

		return all_passed;
	}

}

#endif /* STD_TEST_MON_H_ */




