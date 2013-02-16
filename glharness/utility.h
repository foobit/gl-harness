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

#include <utility>
#include <vector>
#include <algorithm>

namespace glh {

template <class T>
class scope_array
{
	scope_array(scope_array&);
	scope_array& operator=(scope_array const&);
public:
	scope_array() :
		m_array(nullptr),
		m_size(0)
	{}

	scope_array(int size) :
		m_array(nullptr),
		m_size(0)
	{
		realloc(size);
	}

	scope_array(scope_array&& rhs) :
		m_array(nullptr),
		m_size(0)
	{
		swap(rhs);
	}

	~scope_array()
	{
		if (m_array)
		{
			delete[] m_array;
		}
	}

	scope_array& operator=(scope_array&& rhs)
	{
		swap(rhs);
		return *this;
	}

	void swap(scope_array& rhs)
	{
		std::swap(m_array, rhs.m_array);
		std::swap(m_size, rhs.m_size);
	}

	void realloc(int size)
	{
		if (m_array)
		{
			delete[] m_array;
		}
		m_array = new T[size];
		m_size = size;
		clear();
	}

	void clear()
	{
		memset(m_array, 0, sizeof(T) * m_size);
	}

	operator T* ()
	{
		return m_array;
	}

	operator const T* () const
	{
		return m_array;
	}

	const T& operator[] (int index) const
	{
		return m_array[index];
	}

	T& operator[] (int index)
	{
		return m_array[index];
	}

	T* get() const
	{
		return m_array;
	}

	int get_size() const
	{
		return m_size;
	}

private:
	T*		m_array;
	int		m_size;
};

template<class T, class U> void erase_range(const T& t, U& u)
{
	for (auto i : t)
	{
		auto j = std::find(u.begin(), u.end(), i);
		if (j != u.end())
		{
			u.erase(j);
		}
	}
}

} // namespace glh
