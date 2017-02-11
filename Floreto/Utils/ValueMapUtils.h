#ifndef __FLORETO_VALUEMAPTUTILS_H__
#define __FLORETO_VALUEMAPTUTILS_H__

#include <cocos2d.h>

namespace Floreto
{

	class ValueMapUtils
	{
	public:
		static cocos2d::Value tryGetValue(const std::string &key, const cocos2d::ValueMap &data, const cocos2d::Value &fallback = cocos2d::Value::Null)
		{
			auto it = data.find(key);
			if (it != data.end())
				return it->second;
			else
				return fallback;
		}

		static cocos2d::Value tryGetValue(const std::string &key, const cocos2d::ValueMap &data, const std::string &extraKey, const cocos2d::ValueMap *extra, const cocos2d::Value &fallback = cocos2d::Value::Null)
		{
			if (extra)
			{
				auto it = extra->find(extraKey);
				if (it != extra->end())
					return it->second;
			}

			return tryGetValue(key, data, fallback);
		}

		static cocos2d::Vec2 tryGetVec2(const char *key, const cocos2d::ValueMap &data, const cocos2d::Vec2 &fallback = cocos2d::Vec2::ZERO)
		{
			auto value = ValueMapUtils::tryGetValue(key, data);
			if (value.getType() == cocos2d::Value::Type::VECTOR)
			{
				const auto &arr = value.asValueVector();
				if (arr.size() != 2)
					return fallback;

				cocos2d::Vec2 vec;
				vec.x = arr.at(0).asFloat();
				vec.y = arr.at(1).asFloat();
				return vec;
			}

			return fallback;
		}

		static cocos2d::Vec2 tryGetVec2(const char *key, const cocos2d::ValueMap &data, const char *extraKey, const cocos2d::ValueMap *extra, const cocos2d::Vec2 &fallback = cocos2d::Vec2::ZERO)
		{
			auto value = ValueMapUtils::tryGetValue(key, data, extraKey, extra);
			if (value.getType() == cocos2d::Value::Type::VECTOR)
			{
				const auto &arr = value.asValueVector();
				if (arr.size() != 2)
					return fallback;

				cocos2d::Vec2 vec;
				vec.x = arr.at(0).asFloat();
				vec.y = arr.at(1).asFloat();
				return vec;
			}

			return fallback;
		}

		static std::string tryGetString(const std::string &key, const cocos2d::ValueMap &data, std::string fallback = std::string())
		{
			auto it = data.find(key);
			if (it != data.end())
				return it->second.asString();
			else
				return fallback;
		}

		static std::string tryGetString(const std::string &key, const cocos2d::ValueMap &data, const std::string &extraKey, const cocos2d::ValueMap *extra, std::string fallback = std::string())
		{
			if (extra)
			{
				auto it = extra->find(extraKey);
				if (it != extra->end())
					return it->second.asString();
			}

			return tryGetString(key, data, fallback);
		}

		static int tryGetInt(const std::string &key, const cocos2d::ValueMap &data, int fallback = 0)
		{
			auto it = data.find(key);
			if (it != data.end())
				return it->second.asInt();
			else
				return fallback;
		}

		static int tryGetInt(const std::string &key, const cocos2d::ValueMap &data, const std::string &extraKey, const cocos2d::ValueMap *extra, int fallback = 0)
		{
			if (extra)
			{
				auto it = extra->find(extraKey);
				if (it != extra->end())
					return it->second.asInt();
			}

			return tryGetInt(key, data, fallback);
		}

		static float tryGetFloat(const std::string &key, const cocos2d::ValueMap &data, float fallback = 0)
		{
			auto it = data.find(key);
			if (it != data.end())
				return it->second.asFloat();
			else
				return fallback;
		}

		static float tryGetFloat(const std::string &key, const cocos2d::ValueMap &data, const std::string &extraKey, const cocos2d::ValueMap *extra, float fallback = 0)
		{
			if (extra)
			{
				auto it = extra->find(extraKey);
				if (it != extra->end())
					return it->second.asFloat();
			}

			return tryGetFloat(key, data, fallback);
		}

		static bool tryGetBool(const std::string &key, const cocos2d::ValueMap &data, bool fallback = false)
		{
			auto it = data.find(key);
			if (it != data.end())
				return it->second.asBool();
			else
				return fallback;
		}

		static bool tryGetBool(const std::string &key, const cocos2d::ValueMap &data, const std::string &extraKey, const cocos2d::ValueMap *extra, bool fallback = false)
		{
			if (extra)
			{
				auto it = extra->find(extraKey);
				if (it != extra->end())
					return it->second.asBool();
			}

			return tryGetBool(key, data, fallback);
		}
	};

}

#endif//__FLORETO_VALUEMAPTUTILS_H__
