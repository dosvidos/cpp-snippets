#include <typeinfo>
#include <iostream>
#include <tr1/memory>

namespace v_ass {
using namespace std;

class A {
	friend ostream& operator<<(ostream& os, const A&);
public:
	A(int val = 0): a_m(val) { }
	virtual A& operator=(const A& rhs) {
		cout << "virtual A& operator=(const A& rhs) called" << endl;
		if (this != &rhs) {
			a_m = rhs.a_m;
		}
		return *this;
	}
private:
	int a_m;
};

class B : public A {
	friend ostream& operator<<(ostream& os, const B&);	
public:
	B(int val = 0): A(val), b_m(val) { }
	virtual B& operator=(const A& rhs) {
		cout << "virtual B& operator=(const A& rhs) called" << endl;
		if (this != &rhs) {
			try {
				const B& donor = dynamic_cast<const B&>(rhs);
				b_m = donor.b_m;
				A::operator=(rhs);
			}
			catch(std::bad_cast& ex) {
				cout << "virtual B& operator=(const A& rhs) call failed, different type provided" << endl;
				ex;
			}
		}
		return *this;
	}
private:
	B& operator=(const B& rhs);
	int b_m;
};

class C : public A {
	friend ostream& operator<<(ostream& os, const C&);
public:
	C(int val = 0): A(val), c_m(val) { }
	// co-variant return types
	virtual C& operator=(const A& rhs) {
		cout << "virtual C& operator=(const A& rhs) called" << endl;
 		if (this != &rhs) {
			try {
				const C& donor = dynamic_cast<const C&>(rhs);
				c_m = donor.c_m;
				A::operator=(rhs);
			}
			catch(std::bad_cast& ex) {
				cout << "virtual C& operator=(const A& rhs) call failed, different type provided" << endl;
				ex;
			}
		}
		return *this;
	}
private:
	C& operator=(const C& rhs);
	int c_m;
};

ostream& operator<< (ostream& os, const A& s) {
  os << "A::a_m = " << s.a_m;
  return os;
}

ostream& operator<< (ostream& os, const B& s) {
  os << static_cast<const A&>(s) << " B::b_m = " << s.b_m;
  return os;
}

ostream& operator<< (ostream& os, const C& s) {
  os << static_cast<const A&>(s) << " C::c_m = " << s.c_m;
  return os;
}

}

int main(int argc, char** argv) {
	using namespace v_ass;
	A a(1);
	B b(2);
	tr1::shared_ptr<A> a_b(new B(3));
	tr1::shared_ptr<B> b_b(new B(4));
	tr1::shared_ptr<A> a_c(new C(5));
	cout << "a = b" << endl;
	a = b; // slicing occurs
	cout << a << endl;
	a = A(1);
	cout << "b = a" << endl;
	b = a;	
	cout << b << endl;
	cout << "b = *a_b" << endl;
	b = *a_b;
	cout << b << endl;
	//b = *b_b;
	cout << "b = *a_c" << endl;
	b = *a_c;
	cout << "*a_c = b" << endl;
	*a_c = b;
	return 0;
}