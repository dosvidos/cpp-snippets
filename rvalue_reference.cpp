#include <iostream>
#include <string>
#include <utility>
#include <vector>
#include <tr1/random>
#include <algorithm>
#include <ctime>

static int Copies = 0;
class Pod
{
public:
	// constructor
	Pod() : m_index(0) 
	{ 
		m_buffer.resize(1000); // holds 1k of data
	}
	
	Pod(Pod const & pod)
		: m_index(pod.m_index)
		, m_buffer(pod.m_buffer)
	{
		++Copies;
	}
	
	Pod &operator=(Pod const &pod)
	{
		m_index = pod.m_index;
		m_buffer = pod.m_buffer;
		++Copies;
	}
	
	Pod(Pod && pod)
	{
		m_index = pod.m_index;
		// give a hint to tell the library to "move" the vector if possible
		m_buffer = std::move(pod.m_buffer);
		++Copies;
	}

	Pod & operator=(Pod && pod)
	{
		m_index = pod.m_index;
		// give a hint to tell the library to "move" the vector if possible
		m_buffer = std::move(pod.m_buffer);
		++Copies;
	}
	int m_index;
	std::vector<unsigned char> m_buffer;
};

struct PodGreaterThan
{
	bool operator() (Pod const & lhs, Pod const & rhs) const
	{
		if(lhs.m_index > rhs.m_index) { return true; }
		return false;
	}
};



int main()
{
	std::clock_t start;
	std::clock_t end;

	std::vector<Pod> manyPods(1000000);
	std::tr1::mt19937 eng; 
	std::tr1::uniform_int<int> unif(1, 0x7fffffff);
	for (std::size_t i = 0; i < manyPods.size(); ++i)
	{		
		manyPods[i].m_index = unif(eng);
	}
	
	start = std::clock();
	
	std::sort(manyPods.begin(), manyPods.end(), PodGreaterThan());
	
	end = std::clock();
	std::cout<<"The operation took "
		<< ( double ( end ) - start ) / CLOCKS_PER_SEC
		<<" seconds\n";
		
	std::cout<<"Copies "<< Copies << std::endl;
	return 0;
}