#include <stdio.h>
int main() {
  char s[5];
  s[0] = 'C'; s[1] = s[2] = '+'; s[3] = s[4] = 0;
  s[sizeof(' ') ^ 5] = 0;
  printf("%s\n", s);
  return 0;
}

#include <iostream>
using namespace std;

class A {
public:
    virtual void Foo (int n = 10) {
        cout << "A::Foo, n = " << n << endl;
    }
};

class B : public A {
public:
    virtual void Foo (int n = 20) {
        cout << "B::Foo, n = " << n << endl;
    }
};

int main() {
    A * pa = new B ();
    pa->Foo ();

    return 0;
}


