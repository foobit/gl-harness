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

#pragma once

#include <string>

namespace glh {
namespace str {
	
int compare(const std::string& a, const std::string& b, bool ignorecase);
void replace(std::string& s, int index, const char c);

int next_utf8(const std::string& s, std::string::const_iterator& it);
int prev_utf8(const std::string& s, std::string::const_iterator& it);

bool to_bool(const std::string& s);

std::string to_lower(const std::string& s);
std::wstring to_wide(const std::string& s);

std::string format(const char* s, ...);
std::string from_wchar(const wchar_t* ws);

} // namespace str
} // namespace glh
