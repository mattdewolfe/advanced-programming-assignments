namespace Advanced2D
{
#ifndef CSINGLETON_H
#define CSINGLETON_H
template <typename T>
	class CSingleton
	{
	public:
		static T* GetInstance();
	protected:
		CSingleton(){}
		virtual ~CSingleton() {}

	private:
		static T* CreateInstance();
		static T* m_Instance;

	};

	template <typename T>
	T* CSingleton<T>::m_Instance = 0;

	template <typename T>
	T* CSingleton<T>::GetInstance()
	{
		if (CSingleton::m_Instance == 0)
		{
			CSingleton::m_Instance = CreateInstance();
		}
		return CSingleton::m_Instance;
	}

	template <typename T>
	T* CSingleton<T>::CreateInstance()
	{
		return new T();
	}
}
#endif