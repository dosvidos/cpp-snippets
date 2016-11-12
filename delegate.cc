#include <iostream>
#include <assert.h>
#include <typeinfo>       // operator typeid

//  Контейнер для хранения до 2-х аргументов.
struct NIL {};
class IArguments { public: virtual ~IArguments() {} };
template< class T1 = NIL, class T2 = NIL >
  class Arguments : public IArguments
{
  public: Arguments() {}
  public: Arguments( T1 i_arg1 ) :
    arg1( i_arg1 ) {}
  public: Arguments( T1 i_arg1, T2 i_arg2 ) :
    arg1( i_arg1 ), arg2( i_arg2 ) {}
  public: T1 arg1; T2 arg2;
};

//  Контейнер для хранения указателя на метод.
class IContainer { public: virtual void Call( IArguments* ) = 0; };
template< class T, class M > class Container : public IContainer {};

//  Специализация для метода без аргументов.
template< class T >
  class Container< T, void (T::*)(void) > : public IContainer
{
  typedef void (T::*M)(void);
  public: Container( T* c, M m ) : m_class( c ), m_method( m ) { std::cout << "no args: " << typeid(m).name()  << std::endl; }
  private: T* m_class; M m_method;
  public: void Call( IArguments* i_args )
  {
    (m_class->*m_method)();
  }
};

//  Специализация для метода с одним аргументом.
template< class T, class A1 >
  class Container< T, void (T::*)(A1) > : public IContainer
{
  typedef void (T::*M)(A1);
  typedef Arguments<A1> A;
  public: Container( T* c, M m ) : m_class( c ), m_method( m ) { std::cout << "1 arg: " << typeid(m).name() << std::endl; }
  private: T* m_class; M m_method;
  public: void Call( IArguments* i_args )
  {
    A* a = dynamic_cast< A* >( i_args );
    assert( a );
    if( a ) (m_class->*m_method)( a->arg1 );
  }
};

//  Специализация для метода с двумя аргументами
template< class T, class A1, class A2 >
  class Container< T, void (T::*)(A1,A2) > : public IContainer
{
  typedef void (T::*M)(A1,A2);
  typedef Arguments<A1,A2> A;
  public: Container( T* c, M m ) : m_class( c ), m_method( m ) { std::cout << "2 args" << std::endl; }
  private: T* m_class; M m_method;
  public: void Call( IArguments* i_args )
  {
    A* a = dynamic_cast< A* >( i_args );
    assert( a );
    if( a ) (m_class->*m_method)( a->arg1, a->arg2 );
  }
};

//  Собственно делегат.
class Delegate
{
public:

  Delegate() : m_container( 0 ) {}
  ~Delegate() { if( m_container ) delete m_container; }

  template< class T, class U > void Connect( T* i_class, U i_method )
  {
    if( m_container ) delete m_container;
    m_container = new Container< T, U >( i_class, i_method );
  }

  void operator()()
  {
      Arguments<> arg;
    m_container->Call( & arg );
  }

  template< class T1 > void operator()( T1 i_arg1 )
  {
    Arguments< T1 > arg( i_arg1 ); 
    m_container->Call( &arg );
  }

  template< class T1, class T2 > void operator()( T1 i_arg1, T2 i_arg2 )
  {
    m_container->Call( & Arguments< T1, T2 >( i_arg1, i_arg2 ) );
  }

private:
  IContainer* m_container;
};

class Victim { public: void Foo() {} void Bar( int ) {} };

int main()
{
  Victim test_class;
  Delegate test_delegate;
  test_delegate.Connect( & test_class, & Victim::Foo );
  test_delegate();
  test_delegate.Connect( & test_class, & Victim::Bar );
  test_delegate( 10 );
  return 0;
}