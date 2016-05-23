#include<iostream>
#include<stdio.h>
using namespace std;

class A
{
public:
	A(int x):x(x)
	{
		cout<<"A's constructor"<<endl;
		aaa=8;
	}
	int x;
	static int aaa;
};
int A::aaa=6;

class B
{
public:
	B(int y)
	{
		this->y = y;
		cout<<"B's constructor"<<endl;
	}
	virtual void vvv()
	{
		cout<<"B:vvv"<<endl;
	}
	int y;
};

class C: public B,public A
{
public:
	C(int a, int b, int c):z(c),A(a),B(b)
	{
		
		
		cout<<"z="<<z<<endl;
		cout<<"C's constructor"<<endl;
	}
	virtual void vvv()
	{
		cout<<"C:vvv"<<endl;
	}
	virtual void eee()
	{
		cout<<"C:eee"<<endl;
	}
	int z;
};
class D:public C
{
public:
	D():C(1,2,4)
	{
	}
	virtual void eee()
	{
		C::eee();
		cout<<"D:eee"<<endl;
	}
	int d;
};
int main()
{
	A a(8);
	B b(5);
	C c(1,2,3);
	D d;
	cout<<"x = "<<c.x<<"y="<<c.y<<"z="<<c.z<<endl;
	printf("sizeof A = %d, sizof B = %d, sizeof c=%d\n", sizeof(A), sizeof(B), sizeof(C));
	printf("aaa=%d\n", A.aaa);
	c.vvv();
	B &bref = c;
	bref.vvv();
	C &cref = d;
	cref.eee();
	return 0;
}