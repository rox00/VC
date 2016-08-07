// myConsoleTest.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include <iostream>
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

int _tmain(int argc, _TCHAR* argv[])
{
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

