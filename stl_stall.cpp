#include <vector>
#include <iostream>
#include <algorithm>
#include <windows.h>

// Класс Abc нам нужен для того, чтобы сделать у него функцию,
// которая бы никогда не заинлайнилась. Благодаря этому, не
// инлайнятся вызовы lower_bound в main.
class Abc
{
public:
    virtual ~Abc() {}
    virtual int compare(int a, int b) { return a < b; }
};

Abc g_abc;
Abc *g_pAbc = &g_abc;

struct MyStruct
{
    int a;

    friend int operator<(const MyStruct &l, const MyStruct &r)
    {
        return g_pAbc->compare(l.a,r.a);
    }
};

// MyPred1 является братом-близнецом std::less.
// Приведена здесь для наглядности.
struct MyPred1
{
    template<typename T1, typename T2>
    int operator()(const T1 &l, const T2 &r) const { return l < r; }
};

// Единственное отличие MyPred2 от MyPred1 -- это наличие члена int a в MyPred2.
struct MyPred2
{
    int a;

    template<typename T1, typename T2>
    int operator()(const T1 &l, const T2 &r) const { return l < r; }
};

void main()
{
    const size_t count = 10*1024*1024;

    // Вектор, в котором будем искать, пустой, чтобы сделать разницу ощутимой.
    std::vector<MyStruct> t;

    LARGE_INTEGER t1,t2,t3;

    // Холостой прогон.
    for(size_t a0 = 0; a0 < count; ++a0)
    {
        std::vector<MyStruct>::iterator i0 = std::lower_bound(t.begin(),t.end(),MyStruct(),MyPred1());
    }

    QueryPerformanceCounter(&t1);

    // Вариант 1.
    for(size_t a1 = 0; a1 < count; ++a1)
    {
        std::vector<MyStruct>::iterator i1 = std::lower_bound(t.begin(),t.end(),MyStruct(),MyPred1());
    }

    QueryPerformanceCounter(&t2);

    // Вариант 2.
    for(size_t a2 = 0; a2 < count; ++a2)
    {
        std::vector<MyStruct>::iterator i2 = std::lower_bound(t.begin(),t.end(),MyStruct(),MyPred2());
    }

    QueryPerformanceCounter(&t3);

    std::cout << t2.QuadPart - t1.QuadPart << " " << t3.QuadPart - t2.QuadPart << "\n";
    std::cout << (double)(t2.QuadPart - t1.QuadPart)/(double)(t3.QuadPart - t2.QuadPart) << "\n";
}