#ifndef __FLORETO_VALUEMAPREADER_H__
#define __FLORETO_VALUEMAPREADER_H__

#include <cocos2d.h>
#include "ValueMapUtils.h"

namespace Floreto
{

	class ValueMapReader
	{
	protected:
		const cocos2d::ValueMap &_object;
		const cocos2d::ValueMap *_extras;

	public:
		ValueMapReader(const cocos2d::ValueMap &mapObject)
			: _object(mapObject), _extras(nullptr)
		{
		}

		ValueMapReader(const cocos2d::ValueMap &mapObject, const cocos2d::ValueMap *extras)
			: _object(mapObject), _extras(extras)
		{
		}

	public:
		const ValueMapReader & read(const std::string &key, int &output, int fallback = 0) const
		{
			output = ValueMapUtils::tryGetInt(key, _object, key, _extras, fallback);
			return *this;
		}

		const ValueMapReader & read(const std::string &key, float &output, float fallback = 0.f) const
		{
			output = ValueMapUtils::tryGetFloat(key, _object, key, _extras, fallback);
			return *this;
		}

		const ValueMapReader & read(const std::string &key, bool &output, bool fallback = false) const
		{
			output = ValueMapUtils::tryGetBool(key, _object, key, _extras, fallback);
			return *this;
		}

		const ValueMapReader & read(const std::string &key, std::string &output, const std::string &fallback = std::string()) const
		{
			output = ValueMapUtils::tryGetString(key, _object, key, _extras, fallback);
			return *this;
		}

		const ValueMapReader & read(const std::string &key, cocos2d::Value &output, const cocos2d::Value &fallback = cocos2d::Value::Null) const
		{
			output = ValueMapUtils::tryGetValue(key, _object, key, _extras, fallback);
			return *this;
		}
	};

}

#endif//__FLORETO_VALUEMAPREADER_H__
