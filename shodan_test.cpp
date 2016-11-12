// cl /O2 /Ox test.cpp
// gcc -O3 -lstdc++ test.cpp
// shodan(at)shodan.ru

#include <stdlib.h>
#include <stdio.h>

#if _MSC_VER
#define USE_WINDOWS 1
#endif

#if USE_WINDOWS
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#else
#include <time.h>
#include <sys/time.h>
#endif

/// time, in seconds
float myTimer ()
{
#if USE_WINDOWS
	// Windows time query
	static float fFreq;
	static INT64 iStart;
	static bool bFirst = true;

	LARGE_INTEGER iLarge;
	if ( bFirst )
	{
		QueryPerformanceFrequency ( &iLarge );
		fFreq = 1.0f/iLarge.QuadPart;

		QueryPerformanceCounter ( &iLarge );
		iStart = iLarge.QuadPart;

		bFirst = false;
	}

	QueryPerformanceCounter ( &iLarge);
	return ( iLarge.QuadPart-iStart )*fFreq;

#else
	// UNIX time query
	static int s_sec = -1, s_usec = -1;
	struct timeval tv;

	if ( s_sec == -1 )
	{
		gettimeofday ( &tv, NULL );
		s_sec = tv.tv_sec;
		s_usec = tv.tv_usec;
	}
	gettimeofday ( &tv, NULL );
	return float(tv.tv_sec-s_sec) + float(tv.tv_usec-s_usec)/1000000.0f;

#endif // USE_WINDOWS
}


struct Test
{
	unsigned int	m_iCounter;
	unsigned int	m_iLimit;
	char			m_sBuffer[1024];

	Test ()
	{
		m_iLimit = sizeof(m_sBuffer);
		for ( int i=0; i<m_iLimit; i++ )
			m_sBuffer[i] = (char)( rand() % 6 );
	}

	const char * Test1 ()
	{
		// skip whitespace
		while ( m_iCounter<m_iLimit && !m_sBuffer[m_iCounter] )
			m_iCounter++;

		// check for eof
		if ( m_iCounter>=m_iLimit )
		{
			m_iCounter = 0;
			return NULL;
		}

		// skip nonwhitespace
		int iRes = m_iCounter;
		while ( m_iCounter<m_iLimit && m_sBuffer[m_iCounter] )
			m_iCounter++;

		return m_sBuffer + iRes;
	}

	const char * Test2 ()
	{
		// skip whitespace
		int iPos = m_iCounter;
		while ( iPos<m_iLimit && !m_sBuffer[iPos] )
			iPos++;

		// check for eof
		if ( iPos>=m_iLimit )
		{
			m_iCounter = 0;
			return NULL;
		}

		// skip nonwhitespace
		int iRes = iPos;
		while ( iPos<m_iLimit && m_sBuffer[iPos] )
			iPos++;

		m_iCounter = iPos;
		return m_sBuffer + iRes;
	}
};


int main ()
{
	Test * pFoo = new Test ();

	float fTimer1 = -myTimer();
	for ( int i=0; i<100000; i++ )
		while ( pFoo->Test1() );
	fTimer1 += myTimer ();

	float fTimer2 = -myTimer();
	for ( int i=0; i<100000; i++ )
		while ( pFoo->Test2() );
	fTimer2 += myTimer ();

	delete pFoo;

	printf ( "test1=%.3f msec\ntest2=%.3f msec\n", 1000.0f*fTimer1, 1000.0f*fTimer2 );
	return 0;
}