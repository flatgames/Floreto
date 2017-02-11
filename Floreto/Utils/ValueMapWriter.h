#ifndef __FLORETO_VALUEMAPWRITER_H__
#define __FLORETO_VALUEMAPWRITER_H__

#include <cocos2d.h>

namespace Floreto
{

	class ValueMapWriter
	{
	protected:
		cocos2d::ValueMap &_object;

	public:
		ValueMapWriter(cocos2d::ValueMap &mapObject)
			: _object(mapObject)
		{
		}

	public:
		ValueMapWriter & write(const std::string &key, int value)
		{
			_object[key] = value;
			return *this;
		}

		ValueMapWriter & write(const std::string &key, float value)
		{
			_object[key] = value;
			return *this;
		}

		ValueMapWriter & write(const std::string &key, bool value)
		{
			_object[key] = value;
			return *this;
		}

		ValueMapWriter & write(const std::string &key, const std::string &value)
		{
			_object[key] = value;
			return *this;
		}

		ValueMapWriter & write(const std::string &key, const cocos2d::Value &value)
		{
			_object[key] = value;
			return *this;
		}

		template<typename T>
		ValueMapWriter & write(const std::string &key, const T &value)
		{
			_object[key] = value;
			return *this;
		}
	};

}

#endif//__FLORETO_VALUEMAPWRITER_H__
