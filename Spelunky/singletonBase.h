#pragma once

template <typename T>
class singletonBase
{
protected:
	static T* singleton; //싱글톤 인스턴스 선언

	singletonBase(void) {};
	~singletonBase(void) {};

public:
	static T* getSingleton(void); //인스턴스 가져온다
	void releaseSingleton(void); //인스턴스 해제한다
};

//싱글톤 초기화
template <typename T>
T* singletonBase<T>::singleton = 0;

//싱글톤 값 가져온다
template <typename T>
T* singletonBase<T>::getSingleton(void)
{
	if (!singleton) singleton = new T;
	return singleton;
}

//싱글톤 값 해제한다
template <typename T>
void singletonBase<T>::releaseSingleton(void)
{
	if (singleton)
	{
		delete singleton;
		singleton = NULL;
	}
}