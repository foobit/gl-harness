/*
	Copyright 2013 Scott Ramsay

	Licensed under the Apache License, Version 2.0 (the "License");
	you may not use this file except in compliance with the License.
	You may obtain a copy of the License at

		http://www.apache.org/licenses/LICENSE-2.0

	Unless required by applicable law or agreed to in writing, software
	distributed under the License is distributed on an "AS IS" BASIS,
	WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
	See the License for the specific language governing permissions and
	limitations under the License.
*/

#include "pch.h"
#include "str.h"
#include "utility.h"

#include <iterator>
#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <algorithm>
#include <vector>

#include "extern/utf8/source/utf8.h"

namespace glh {

int str::compare(const std::string& a, const std::string& b, bool ignorecase)
{
	if (ignorecase)
	{
		return _stricmp(a.c_str(), b.c_str());
	}

	return strcmp(a.c_str(), b.c_str());
}


void str::replace(std::string& s, int index, const char c)
{
	const_cast<char*>(s.c_str())[index] = c;
}


std::string str::format(const char* s, ...)
{
	va_list args;
	va_start(args, s);
	
	std::string result;
	
	int len = vsnprintf(nullptr, 0, s, args);
	if (len > 0)
	{
		// va_end/va_start hack to fix *nix 64bit versions
		va_end(args);
		va_start(args, s);
		
		scope_array<char> tmp(len+1);
		vsnprintf(tmp.get(), tmp.get_size(), s, args);
		result = tmp;
	}
	va_end(args);

	return result;
}


std::string str::to_lower(const std::string& s)
{
	std::string s0(s);
	std::transform(s0.begin(), s0.end(), s0.begin(), tolower);
	return s0;
}


int str::next_utf8(const std::string& s, std::string::const_iterator& it)
{
	return static_cast<int>(utf8::next(it, s.cend()));
}


int str::prev_utf8(const std::string& s, std::string::const_iterator& it)
{
	return static_cast<int>(utf8::prior(it, s.cbegin()));
}


std::string str::from_wchar(const wchar_t* ws)
{
	std::string s;
	const wchar_t* e = ws;
	while (*(e++)) {}
	utf8::utf16to8(ws, e - 1, back_inserter(s));
	return s;
}


std::wstring str::to_wide(const std::string& s)
{
	std::wstring w;

	utf8::utf8to16(s.cbegin(), s.cend(), back_inserter(w));

	return w;
}

} // namespace glh
