/**
 * @file str_template.h
 *
 * @brief A lightweight and fast template-based string generator
 *
 * @author Dahua Lin
 */

#ifdef _MSC_VER
#pragma once
#endif

#ifndef LIGHT_TEST_STR_TEMPLATE_H_
#define LIGHT_TEST_STR_TEMPLATE_H_

#include <stdlib.h>
#include <cstdio>
#include <cstring>
#include <cctype>
#include <string>
#include <vector>
#include <iostream>
#include <stdexcept>

#define LTEST_STR_FORMAT_BUF_SIZE 128

namespace ltest
{

	class str_template_error : std::runtime_error
	{
	public:
		str_template_error(const char *msg)
		: std::runtime_error(msg) { }
	};


	inline bool str_eq(const char *a, const char *b)
	{
		return std::strcmp(a, b) == 0;
	}

	inline bool str_eq(const std::string& a, const char *b)
	{
		return a.compare(b) == 0;
	}

	inline bool str_eq(const std::string& a, const std::string& b)
	{
		return a.compare(b) == 0;
	}


	template<typename T>
	inline std::string sformat(const T& v, const char *fmt)
	{
		char _buf[LTEST_STR_FORMAT_BUF_SIZE];
		char *dst = _buf;

		if (std::snprintf(dst, LTEST_STR_FORMAT_BUF_SIZE, fmt, v) >= 0)
		{
			return std::string(dst);
		}
		else
		{
			size_t len = (size_t)std::snprintf((char*)0, 0, fmt, v) + 2;
			dst = new char[len];
			std::snprintf(dst, len, fmt, v);
			std::string ret(dst);
			delete[] dst;
			return ret;
		}
	}

	class str_builder
	{
	public:
		str_builder()
		: _capacity(256), _len(0)
		{
			_sz = (char*)malloc(sizeof(char) * _capacity);
			clear();
		}

		~str_builder()
		{
			free(_sz);
		}

		void clear()
		{
			_len = 0;
			_sz[0] = '\0';
		}

		str_builder& operator << (const std::string& s)
		{
			size_t new_len = _len + s.length();
			if (new_len >= _capacity) grow(new_len + 1);
			std::memcpy(_sz + _len, s.c_str(), sizeof(char) * s.length());
			_sz[new_len] = '\0';
			_len = new_len;

			return *this;
		}

		const char* get() const
		{
			return _sz;
		}

	private:
		void grow(size_t n)
		{
			size_t c = _capacity * 2;
			while (c < n) c *= 2;
			_sz = (char*)realloc(_sz, sizeof(char) * c);
			_capacity = c;
		}


		str_builder(const str_builder& );
		str_builder& operator = (const str_builder& );

	private:
		char *_sz;
		size_t _capacity;
		size_t _len;
	};


	class str_formatter
	{
	public:
		str_formatter(const std::string& spec)
		{
			_compile(spec.c_str());
		}

		const char* tag() const
		{
			return m_tag.c_str();
		}

		const char* fmt() const
		{
			return m_fmt.c_str();
		}

		bool empty_fmt() const
		{
			return m_fmt.empty();
		}

		template<class S>
		std::string operator() (const S& src) const
		{
			if (m_fmt.empty())
			{
				return src(tag());
			}
			else
			{
				return src(tag(), fmt());
			}
		}

	private:
		void _compile(const std::string& spec)
		{
			size_t i = spec.find_first_of(':');
			if (i == std::string::npos)
			{
				m_tag = trim(spec);
			}
			else if (i > 0 && i < spec.length() - 1)
			{
				m_tag = trim(spec.substr(0, i));
				m_fmt = trim(spec.substr(i + 1));
			}
			else throw str_template_error("Found invalid place holder");
		}

		static std::string trim(const std::string& s)
		{
			const char *l = s.c_str();
			const char *r = s.c_str() + s.length();
			--r;

			while (std::isspace(*l)) ++l;
			while (r > l && std::isspace(*r)) --r;

			return r > l ? std::string(l, (size_t)(r - l + 1)) : std::string();
		}

	private:
		std::string m_tag;
		std::string m_fmt;
	};


	class str_template
	{
	public:
		str_template()
		{ }

		str_template(const char *sz)
		{
			_compile(sz);
		}

		str_template(const std::string& str)
		{
			_compile(str.c_str());
		}

		void compile(const char *sz)
		{
			m_guide.clear();
			m_fixes.clear();
			m_vars.clear();

			_compile(sz);
		}

		void compile(const std::string& str)
		{
			compile(str.c_str());
		}

		bool empty() const
		{
			return m_guide.empty();
		}

		template<class S>
		void generate_to(str_builder& builder, const S& src) const
		{
			std::vector<std::string>::const_iterator it_fix = m_fixes.begin();
			std::vector<str_formatter>::const_iterator it_var = m_vars.begin();

			for (std::vector<bool>::const_iterator it_g = m_guide.begin();
					it_g != m_guide.end(); ++it_g)
			{
				if (*it_g)  // var
				{
					const str_formatter& v = *it_var++;
					builder << v(src);
				}
				else // fix
				{
					builder << *it_fix++;
				}
			}
		}

		void dump()
		{
			std::printf("str_template\n");
			std::vector<std::string>::const_iterator it_fix = m_fixes.begin();
			std::vector<str_formatter>::const_iterator it_var = m_vars.begin();

			for (std::vector<bool>::const_iterator it_g = m_guide.begin();
					it_g != m_guide.end(); ++it_g)
			{
				if (*it_g)  // var
				{
					const str_formatter& v = *it_var++;

					if (v.empty_fmt())
					{
						std::printf("var: (%s)\n", v.tag());
					}
					else
					{
						std::printf("var: (%s, %s)\n", v.tag(), v.fmt());
					}
				}
				else // fix
				{
					const std::string& s = *it_fix++;
					std::printf("fix: %s\n", s.c_str());
				}
			}
		}

	private:
		void _add_fix(const char *b, const char *e)
		{
			if (e > b)
			{
				std::string s = std::string(b, (size_t)(e - b));
				m_guide.push_back(false);
				m_fixes.push_back(s);
			}
		}

		void _add_variable(const char *b, const char *e)
		{
			if (e > b)
			{
				str_formatter v = std::string(b, (size_t)(e - b));
				m_guide.push_back(true);
				m_vars.push_back(v);
			}
		}

		void _compile(const char *sz)
		{
			unsigned int state = 0;

			const char *sz_b = sz;
			const char *var_b = sz;

			while (char c = *sz)
			{
				switch (state)
				{
				case 0:  // normal text running
					if (c == '{') state = 1;
					break;

				case 1:  // variable being open
					if (c == '{')  // start a variable section
					{
						_add_fix(sz_b, sz-1);
						var_b = sz + 1;
						state = 2;
					}
					else state = 0;  // resume to normal state
					break;

				case 2: // variable section started
					if (c == '}')  state = 3;
					break;

				case 3: // variable being closed
					if (c == '}') // close variable
					{
						_add_variable(var_b, sz-1);
						sz_b = sz + 1;
						state = 0;
					}
					break;
				}

				++sz;
			}

			if (state > 1)
				throw str_template_error("Incompleted template string with unclosed variable section.");

			_add_fix(sz_b, sz);
		}

	private:
		std::vector<bool> m_guide;
		std::vector<std::string> m_fixes;
		std::vector<str_formatter> m_vars;
	};


	struct no_template { };

	class template_channel
	{
	public:
		template_channel(std::ostream& out)
		: m_out(out), m_builder(), m_templ(0) { }

		template_channel& operator << (const str_template& templ)
		{
			m_templ = &templ;
			return *this;
		}

		template_channel& operator << (no_template)
		{
			m_templ = 0;
			return *this;
		}

		template<typename S>
		template_channel& operator << (const S& src)
		{
			if (m_templ)
			{
				m_templ->generate_to(m_builder, src);
				m_out << m_builder.get();
				m_builder.clear();
			}

			return *this;
		}

		template_channel& operator << (const char* s)
		{
			m_out << s;
			return *this;
		}

		template_channel& operator << (const std::string& s)
		{
			m_out << s;
			return *this;
		}

	private:
		std::ostream& m_out;
		str_builder m_builder;
		const str_template* m_templ;
	};


}

#endif
