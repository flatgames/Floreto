#ifndef __FLORETO_JSONUTILS_H__
#define __FLORETO_JSONUTILS_H__

#include "../Common.h"

namespace Floreto
{
	class JsonUtils
	{
	public:
		static cocos2d::Value getValueFromJson(const char *json);
		
		static cocos2d::ValueMap getValueMapFromJson(const char *json)
		{
			return getValueFromJson(json).asValueMap();
		}

		static char * getJsonFromValue(const cocos2d::Value &value, bool pretty = false);

#if RAPIDJSON_UNQUOTE_SUPPORTED
		static char * getCompactJsonFromValue(const cocos2d::Value &value, bool pretty = false);
#endif
	};
}

#endif//__FLORETO_JSONUTILS_H__
