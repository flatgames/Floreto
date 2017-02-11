#ifndef __FLORETO_OBJECTFACTORY_H__
#define __FLORETO_OBJECTFACTORY_H__

#include "../Common.h"

namespace Floreto
{
	class ObjectFactory
	{
	public:
		template<typename T>
		static T * create(void)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T();
			if (obj)
				obj->autorelease();

			return obj;
		}

		template<typename T, typename Ta>
		static T * create(const Ta &arg0)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T(arg0);
			if (obj)
				obj->autorelease();

			return obj;
		}

		template<typename T, typename Ta, typename Tb>
		static T * create(const Ta &arg0, const Tb &arg1)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T(arg0, arg1);
			if (obj)
				obj->autorelease();

			return obj;
		}

		template<typename T, typename Ta, typename Tb, typename Tc>
		static T * create(const Ta &arg0, const Tb &arg1, const Tc &arg2)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T(arg0, arg1, arg2);
			if (obj)
				obj->autorelease();

			return obj;
		}

		template<typename T, typename Ta, typename Tb, typename Tc, typename Td>
		static T * create(const Ta &arg0, const Tb &arg1, const Tc &arg2, const Td &arg3)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T(arg0, arg1, arg2, arg3);
			if (obj)
				obj->autorelease();

			return obj;
		}

		template<typename T, typename Ta, typename Tb, typename Tc, typename Td, typename Te>
		static T * create(const Ta &arg0, const Tb &arg1, const Tc &arg2, const Td &arg3, const Te &arg4)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T(arg0, arg1, arg2, arg3, arg4);
			if (obj)
				obj->autorelease();

			return obj;
		}

		template<typename T, typename Ta, typename Tb, typename Tc, typename Td, typename Te, typename Tf>
		static T * create(const Ta &arg0, const Tb &arg1, const Tc &arg2, const Td &arg3, const Te &arg4, const Tf &arg5)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T(arg0, arg1, arg2, arg3, arg4, arg5);
			if (obj)
				obj->autorelease();

			return obj;
		}

	public:
		template<typename T>
		static T * createWithInit(void)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T();
			if (obj && obj->init())
			{
				obj->autorelease();
				return obj;
			}
			else if (obj)
			{
				delete obj;
				obj = nullptr;
			}

			return nullptr;
		}

		template<typename T, typename Ta>
		static T * createWithInit(const Ta &arg0)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T();
			if (obj && obj->init(arg0))
			{
				obj->autorelease();
				return obj;
			}
			else if (obj)
			{
				delete obj;
				obj = nullptr;
			}

			return nullptr;
		}

		template<typename T, typename Ta, typename Tb>
		static T * createWithInit(const Ta &arg0, const Tb &arg1)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T();
			if (obj && obj->init(arg0, arg1))
			{
				obj->autorelease();
				return obj;
			}
			else if (obj)
			{
				delete obj;
				obj = nullptr;
			}

			return nullptr;
		}

		template<typename T, typename Ta, typename Tb, typename Tc>
		static T * createWithInit(const Ta &arg0, const Tb &arg1, const Tc &arg2)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T();
			if (obj && obj->init(arg0, arg1, arg2))
			{
				obj->autorelease();
				return obj;
			}
			else if (obj)
			{
				delete obj;
				obj = nullptr;
			}

			return nullptr;
		}

		template<typename T, typename Ta, typename Tb, typename Tc, typename Td>
		static T * createWithInit(const Ta &arg0, const Tb &arg1, const Tc &arg2, const Td &arg3)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T();
			if (obj && obj->init(arg0, arg1, arg2, arg3))
			{
				obj->autorelease();
				return obj;
			}
			else if (obj)
			{
				delete obj;
				obj = nullptr;
			}

			return nullptr;
		}

		template<typename T, typename Ta, typename Tb, typename Tc, typename Td, typename Te>
		static T * createWithInit(const Ta &arg0, const Tb &arg1, const Tc &arg2, const Td &arg3, const Te &arg4)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T();
			if (obj && obj->init(arg0, arg1, arg2, arg3, arg4))
			{
				obj->autorelease();
				return obj;
			}
			else if (obj)
			{
				delete obj;
				obj = nullptr;
			}

			return nullptr;
		}

		template<typename T, typename Ta, typename Tb, typename Tc, typename Td, typename Te, typename Tf>
		static T * createWithInit(const Ta &arg0, const Tb &arg1, const Tc &arg2, const Td &arg3, const Te &arg4, const Tf &arg5)
		{
			static_assert(std::is_convertible<T, cocos2d::Ref>::value, "T must be a Ref type.");
			auto obj = new T();
			if (obj && obj->init(arg0, arg1, arg2, arg3, arg4, arg5))
			{
				obj->autorelease();
				return obj;
			}
			else if (obj)
			{
				delete obj;
				obj = nullptr;
			}

			return nullptr;
		}
	};
}

#endif//__FLORETO_OBJECTFACTORY_H__
