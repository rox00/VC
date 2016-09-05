#pragma once 
#include <memory> //线程安全使用的头文件 
#ifndef SingletonH 
#define SingletonH 

using namespace std; 

// Base SinglePattern Class 
class SinglePattern 
{ 
protected: 
	virtual void InitialInstance() = 0; 
	virtual void DisposeInstance() = 0; 
}; 

// Thread Safe Class 
class CResGuard 
{ 
public: 
	CResGuard() { m_lGrdCnt = 0; InitializeCriticalSection(&m_cs); } 
	~CResGuard() { DeleteCriticalSection(&m_cs); } 
	// IsGuarded is used for debugging 
	BOOL IsGuarded() const { return(m_lGrdCnt > 0); } 
public: 
	class CGuard 
	{ 
	public: 
		CGuard(CResGuard& rg) : m_rg(rg) { m_rg.Guard(); }; 
		~CGuard() { m_rg.Unguard(); } 
	protected: 
		CResGuard& m_rg; 
	}; 

private: 
	void Guard() { EnterCriticalSection(&m_cs); m_lGrdCnt++; } 
	void Unguard() { m_lGrdCnt--; LeaveCriticalSection(&m_cs); } 
		
	// Guard/Unguard can only be accessed by the nested CGuard class. 
	friend class CResGuard::CGuard; 

private: 
	CRITICAL_SECTION m_cs; 
	long m_lGrdCnt; // # of EnterCriticalSection calls 
}; 

// Singleton Container 
template <class T> 
class Singleton 
{ 
	friend class auto_ptr<T>; 
protected: 
	static auto_ptr<T> m_Instance; 
	static CResGuard _rs; 
protected: 
	Singleton() {}; 
	virtual ~Singleton() {}; 
public: 
	static T &GetInstance() 
	{ 
		CResGuard::CGuard gd(_rs); 
		if(m_Instance.get() == NULL) 
		{ 
			m_Instance = auto_ptr<T>(new T());
		}
		return *m_Instance;
	} 
};

template <class T> CResGuard Singleton<T>::_rs; 
template<class T> auto_ptr<T> Singleton<T>::m_Instance; 

// MACRO of Singleton Pattern to use 
#define DECLARE_SINGLETON(type) \
friend class auto_ptr<type>; \
friend class Singleton<type>; \
public: \
	static type &GetInstance() \
	{ \
		return Singleton<type>::GetInstance(); \
	} \
protected: \
	type() \
	{ \
		InitialInstance(); \
	} \
	virtual ~type() \
	{ \
		DisposeInstance(); \
	}
#endif 
