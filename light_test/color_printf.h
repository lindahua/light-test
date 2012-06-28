/*
 * @file color_printf.h
 *
 * Helpers for printing colored text to standard output
 *
 * @author Dahua Lin
 */

#ifndef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_COLOR_PRINTF_H_
#define LIGHT_TEST_COLOR_PRINTF_H_

#include "base.h"
#include <stdarg.h>
#include <cstdio>

#if (defined(_WIN32) || defined(_WIN64))
#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#define NOGDI
#include <windows.h>
#undef WIN32_LEAN_AND_MEAN
#undef NOMINMAX
#undef NOGDI
#endif


namespace ltest
{

	enum color_t
	{
		LTCOLOR_BLACK,
		LTCOLOR_RED,
		LTCOLOR_GREEN,
		LTCOLOR_BROWN,
		LTCOLOR_BLUE,
		LTCOLOR_PURPLE,
		LTCOLOR_CYAN,
		LTCOLOR_GRAY
	};


	inline int term_color_code(color_t color)
	{
		switch (color)
		{
		case LTCOLOR_BLACK:  return 30;
		case LTCOLOR_RED:    return 31;
		case LTCOLOR_GREEN:  return 32;
		case LTCOLOR_BROWN:  return 33;
		case LTCOLOR_BLUE:   return 34;
		case LTCOLOR_PURPLE: return 35;
		case LTCOLOR_CYAN:   return 36;
		case LTCOLOR_GRAY:   return 37;
		}

		return 0;
	}

#if (defined(_WIN32) || defined(_WIN64))

	inline WORD windows_color_code(color_t color)
	{
		switch (color)
		{
		case LTCOLOR_RED:    return FOREGROUND_RED;
		case LTCOLOR_GREEN:  return FOREGROUND_GREEN;
		case LTCOLOR_BROWN:  return FOREGROUND_RED | FOREGROUND_GREEN;
		case LTCOLOR_BLUE:   return FOREGROUND_BLUE;
		case LTCOLOR_PURPLE: return FOREGROUND_RED | FOREGROUND_BLUE;
		case LTCOLOR_CYAN:   return FOREGROUND_GREEN | FOREGROUND_BLUE;
		case LTCOLOR_GRAY:   return FOREGROUND_RED | FOREGROUND_GREEN | FOREGROUND_BLUE;
		}

		return 0;
	}
#endif

	inline void printf_with_color(color_t color, const char *fmt, ...)
	{
		  va_list args;
		  va_start(args, fmt);

#if (defined(_WIN32) || defined(_WIN64))
		  // get the handle to the standard console output
		  HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);

		  // flush current IO buffer
		  ::FlushConsoleInputBuffer(hConsole);

		  // preserve current console info
		  CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		  ::GetConsoleScreenBufferInfo(hConsole, &buffer_info);
		  const WORD original_attr = buffer_info.wAttributes;

		  // set new attributes
		  ::SetConsoleTextAttribute(hConsole, windows_color_code(color));

		  // print text
		  std::vprintf(fmt, args);

		  // restore original attribute
		  ::SetConsoleTextAttribute(hConsole, original_attr);

#else
		  std::printf("\033[0;%dm", term_color_code(color)); // set color
		  std::vprintf(fmt, args);
		  std::printf("\033[m");  // reset
#endif
		  va_end(args);
	}


	inline void printf_with_color_bold(color_t color, const char *fmt, ...)
	{
		  va_list args;
		  va_start(args, fmt);

#if (defined(_WIN32) || defined(_WIN64))
		  // get the handle to the standard console output
		  HANDLE hConsole = ::GetStdHandle(STD_OUTPUT_HANDLE);

		  // flush current IO buffer
		  ::FlushConsoleInputBuffer(hConsole);

		  // preserve current console info
		  CONSOLE_SCREEN_BUFFER_INFO buffer_info;
		  ::GetConsoleScreenBufferInfo(hConsole, &buffer_info);
		  const WORD original_attr = buffer_info.wAttributes;

		  // set new attributes
		  ::SetConsoleTextAttribute(hConsole, windows_color_code(color) | FOREGROUND_INTENSITY);

		  // print text
		  std::vprintf(fmt, args);

		  // restore original attribute
		  ::SetConsoleTextAttribute(hConsole, original_attr);
#else
		  std::printf("\033[0;%d;1m", term_color_code(color)); // set color
		  std::vprintf(fmt, args);
		  std::printf("\033[m");  // reset
#endif
		  va_end(args);
	}

}

#endif 
