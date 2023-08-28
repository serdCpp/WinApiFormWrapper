#pragma once
#ifndef SERD_STRING_H
#define SERD_STRING_H

#include <string>
#include <iostream>

namespace serd {

#if defined(_UNICODE) || defined(UNICODE)
	typedef std::wstring  string_basic;
	typedef wchar_t       char_basic;
	typedef std::string   string_addition;
	typedef char          char_addition;

	typedef std::ostream  stream_addition;
#else
	typedef std::wstring  string_addition;
	typedef wchar_t       char_addition;
	typedef std::string   string_basic;
	typedef char          char_basic;
	typedef std::wostream stream_addition;
#endif // UNICODE

	class STRING : public string_basic {
	public:
		using string_basic::string_basic;
		using string_basic::operator=;
		using string_basic::operator+=;
		using string_basic::operator[];
		//using string_basic::pointer;


		STRING() : string_basic() {};
		STRING(char_basic* str) : string_basic(str) {};
		STRING(const char_addition* str) {
			*this = convert(string_addition(str));
		};
		STRING(const string_addition& str) {
			*this = convert(str);
		};
		
		STRING operator +=(const char_addition* rhs) {
			*this += convert(string_addition(rhs));
			return *this;
		};
		STRING operator  =(const char_addition* rhs) {
			*this = convert(string_addition(rhs));
			return *this;
		};
		
		friend stream_addition& operator<<(stream_addition& outstream, const STRING& str) {
			outstream << str.convert();
			return outstream;
		};

		STRING operator +(const char_addition* rhs) {
			STRING temp(*this);
			temp+= STRING(rhs);
			return temp;
		};
		friend STRING operator+(const char_addition* lhs, const STRING& rhs) {
			STRING temp(lhs);
			temp += rhs;
			return temp;
		};

		string_addition convert() const {
			return string_addition(this->begin(), this->end());
		};

		static string_addition convert(const string_basic& str) {
			return string_addition(str.begin(), str.end());
		};
		static string_basic	convert(const string_addition& str) {
			return string_basic(str.begin(), str.end());
		};

		inline std::string toString() const {
#if defined(_UNICODE) || defined(UNICODE)
			return this->convert();
#else
			return *this;
#endif // UNICODE
		};
		inline std::wstring toWString() const {
#ifdef UNICODE
			return *this;
#else
			return this->convert();
#endif // UNICODE
		};
	};

};

#endif // SERD_STRING_H
