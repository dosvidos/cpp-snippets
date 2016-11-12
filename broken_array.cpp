// code_quiz.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <vector>

template<typename T>
class Array
{
public:
	Array(unsigned int size, int defValue)
	{
		m_intAr.assign(size, defValue);
	}
	Array(unsigned int size)
	{
		m_intAr.reserve(size);
	}
	T& operator[] (unsigned int index)
	{
		return m_intAr[index];
	}
	const T& operator[] (unsigned int index) const
	{
		return m_intAr[index];
	}
	const unsigned int size(void) const
	{
		return m_intAr.size();
	}
	friend bool operator==(const Array<T>& lhs, const Array<T>& rhs)
	{
		//bool isArEqual = false;
		//if (lhs.size() == rhs.size())
		//{
		//	for(unsigned int idx = 0; idx < lhs.size(); ++idx)
		//		if(lhs[idx] != rhs[idx])
		//			return isArEqual;

		//	isArEqual = true;
		//}

		//return isArEqual;
		if (lhs.size() == rhs.size())
		{
			for(unsigned int idx = 0; idx < lhs.size(); ++idx)
				if(lhs[idx] != rhs[idx])
					goto FALSE;

			return true;
		}
FALSE:
		return false;
	}
private:
	std::vector<T> m_intAr;
};

int _tmain(int argc, _TCHAR* argv[])
{
	return 0;
}

