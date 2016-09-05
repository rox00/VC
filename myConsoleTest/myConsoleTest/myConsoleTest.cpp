// myConsoleTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
#include <wtypes.h>
using namespace std;
/*************构造函数不能为虚函数**********************/
class MyClass
{
public:
	/*virtual*/ MyClass(){};
	~MyClass(){};

private:

};
/************************************************/

/*************构造函数 析构函数 中调用虚函数**********************/
class Base
{
public:
	Base()
	{////构造函数中 修改虚表为本类的表
		Fuction();
	}

	~Base()
	{////析构函数中 修改虚表为本类的表
		Fuction();
	}

	virtual void Fuction()
	{
		cout << "Base::Fuction" << endl;
	}

	virtual void BaseBase()
	{
		cout << "Base::BaseBase" << endl;
	}


};

class A : public Base
{
public:
	A()
	{
		Fuction();
		AAAA();
	}

	~A()
	{
		int a = 0;
		a = 9;
		Fuction();
	}

	virtual void Fuction()
	{
		cout << "A::Fuction" << endl;
	}

	virtual void AAAA()
	{////如果虚函数不存在于基类中，则不存在于虚表中
		cout << "A::AA" << endl;
	}
};
/************************************************/

#define DEFINETEST 10;
int _tmain(int argc, _TCHAR* argv[])
{
	{
		int aa[5] = { 1, 2, 3, 4, 5 };
		int *ptr = (int *)(&aa + 1);
	}
	{
		BYTE aa[5] = { 66, 50, '\0', 71, 72 };
		char bb[5] = { 66, 50, '\0', 71, 72 };
		int sizea = sizeof(aa);
		int sizeb = sizeof(bb);

		int lenb = strlen(bb);

		lenb = 0;

		char *p = "string";

		//*p = '2';
		//cout << p << endl;
	}

	{
		#define SQUARE(a) ((a)*(a))
		int a = 5;
		int b;
		b = SQUARE(a++);
		b = 0;
	}

	{
		int a = 5;
		int b;
		int c = a++ + a++;
		b = 0;
	}
	{
		typedef union { short i; short k[1]; char c; } DATE;
		struct data { short cat; DATE cow; double dog; } too;
		int a = sizeof(DATE);
		int b = sizeof(too);
		a = 0;
	}
	//string str = DEFINETEST;
	{// 栈分配内存由高到低，堆分配内存由低到高。

		char array1[10];
		char array2[10];

		char *pArray3 = new char[10];
		char *pArray4 = new char[10];
		int dd3 = (int)&pArray3;
		int dd4 = (int)&pArray4;
		dd4 = 0;
	}

	//MyClass myclass;
	Base *pa = new A();
	pa->Fuction();
	delete pa;
	A a;
	return 0;
}

