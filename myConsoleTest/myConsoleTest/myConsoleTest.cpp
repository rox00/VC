// myConsoleTest.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include <iostream>
#include <wtypes.h>
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

