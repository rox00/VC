// myConsoleTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
using namespace std;
/*************���캯������Ϊ�麯��**********************/
class MyClass
{
public:
	/*virtual*/ MyClass(){};
	~MyClass(){};

private:

};
/************************************************/

/*************���캯�� �������� �е����麯��**********************/
class Base
{
public:
	Base()
	{////���캯���� �޸����Ϊ����ı�
		Fuction();
	}

	~Base()
	{////���������� �޸����Ϊ����ı�
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
	{////����麯���������ڻ����У��򲻴����������
		cout << "A::AA" << endl;
	}
};
/************************************************/

int _tmain(int argc, _TCHAR* argv[])
{
	{// ջ�����ڴ��ɸߵ��ͣ��ѷ����ڴ��ɵ͵��ߡ�

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

