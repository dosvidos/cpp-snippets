#include <iostream>
#include <exception>

static unsigned long g_i = 0;

struct foo
{
    foo(const char* name)
    try : _name((++g_i, name))
    { }
    catch (std::exception&)
    {
        throw;
    }
    
    // function overloading for different array sizes
    static void fiz(int a[]) {};
    // compilation error - redifinition of previously declared function
    //static void fiz(int * a) {};
    //static void fiz(int a[3]) {};
    
    static void baz(int (&a)[2]) {};
    static void baz(int (&a)[10]) {};

    void bar() const& {
        std::cout << "bar::const&\n";
    }
    void bar() && {
        std::cout << "bar::&&\n";
    }

    std::string _name;
};


int main(){
    // this is legal - just a simple label followed by comment
    http://google.com
    foo{"foo1"}.bar(); //This outputs "bar::&&\n"
    foo temp{"foo2"};
    temp.bar(); //This outputs "bar::const&\n"

    std::cout << temp._name.c_str() << " " << g_i;

    int a[] = {10, 12};
    foo::fiz(a);
    foo::baz(a);
}
