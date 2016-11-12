// cl /O2 perftest.cpp
// shodan(at)shodan.ru
 
#include <stdlib.h>
#include <stdio.h>
#include <time.h> 
 
#ifdef _WIN32
       #include <windows.h>
        #pragma comment(linker, "/defaultlib:winmm.lib") 
        typedef __int64 int64_t;
#else
        #include <sys/time.h>
        #define __forceinline inline
#endif
 
/// time since startup, in microseconds
int64_t sphMicroTimer()
{
#ifdef _WIN32
        // Windows time query
        static int64_t iStart = 0;
        static int64_t iFreq = 0;
 
        LARGE_INTEGER iLarge;
        if ( !iFreq )
        {
                QueryPerformanceFrequency ( &iLarge ); iFreq = iLarge.QuadPart;
                QueryPerformanceCounter ( &iLarge ); iStart = iLarge.QuadPart;
        }
 
        QueryPerformanceCounter ( &iLarge);
        return ( iLarge.QuadPart-iStart )*1000000/iFreq;
 
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
        return int64_t(tv.tv_sec-s_sec)*int64_t(1000000) + int64_t(tv.tv_usec-s_usec);
#endif // USE_WINDOWS
}
 
struct Locator
{
        int m_iOffset;
        bool m_bPart;
};
 
struct Foo
{
        int * m_pParts[2];
 
        __forceinline int Getter1 ( const Locator & l ) const
        {
                return l.m_bPart
                        ? m_pParts[1][l.m_iOffset]
                        : m_pParts[0][l.m_iOffset];
        }
 
        __forceinline int Getter2 ( const Locator & l ) const
        {
                return m_pParts[l.m_bPart][l.m_iOffset];
        }
};
 
__forceinline int Marker()
{
#ifdef _WIN32
        __asm
        {
                or eax,eax
                or ebx,ebx
                or ecx,ecx
        };
#else
        asm(“orl %eax,%eax”);
        asm(“orl %ebx,%ebx”);
        asm(“orl %ecx,%ecx”);
#endif
        return 0;
}
 
int main ()
{
#ifdef _WIN32
        timeBeginPeriod ( 1 );
#endif

       int dData1[] = { 1, 2, 3 };
       int dData2[] = { 4, 5, 6 };
 
        Locator l;
        srand ( time(NULL) );
        l.m_iOffset = rand() % 3;
        l.m_bPart = rand() % 2;
 
        volatile int NRUNS = 1000000;
        volatile int NOBJECTS = 100;
 
        Foo * f = new Foo [ NOBJECTS ];
        for ( int i=0; i<NOBJECTS; i++ )
        {
                f[i].m_pParts[0] = dData1;
                f[i].m_pParts[1] = dData2;
        }
 
        int64_t t1 = -sphMicroTimer();
        int iSum1 = 0;
        Marker();
        for ( int i=0; i<NRUNS; i++ )
                for ( int j=0; j<NOBJECTS; j++ )
                        iSum1 += f[j].Getter1 ( l );
        Marker();
        t1 += sphMicroTimer();
 
        int64_t t2 = -sphMicroTimer();
        int iSum2 = 0;
        Marker();
        for ( int i=0; i<NRUNS; i++ )
                for ( int j=0; j<NOBJECTS; j++ )
                        iSum2 += f[j].Getter2 ( l );
        Marker();
        t2 += sphMicroTimer ();
 
#ifdef _WIN32
        timeEndPeriod ( 1 );
#endif
 
        printf ( "off=%d part=%d\n", l.m_iOffset, l.m_bPart );
        printf ( "sum1=%d time1=%d\n", iSum1, t1 );
        printf ( "sum2=%d time2=%d\n", iSum2, t2 );
} 