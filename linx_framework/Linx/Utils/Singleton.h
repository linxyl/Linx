#pragma once

namespace Linx
{
	template <typename T>
	class Singleton
	{
	public:
		static inline T * Instance()
		{
			static T Inst;
			return &Inst;
		}

	private:
		Singleton() {}
		Singleton(const Singleton<T> &);
		Singleton<T>& operator = (const Singleton<T>&) {};
		~Singleton() {}
	};
}

