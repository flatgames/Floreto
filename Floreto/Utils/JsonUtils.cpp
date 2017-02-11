#include "JsonUtils.h"
#include "../external/json/document.h"
#include "../external/json/writer.h"
#include "../external/json/prettywriter.h"
#if RAPIDJSON_UNQUOTE_SUPPORTED
#include "../external/json/compactwriter.h"
#include "../external/json/prettycompactwriter.h"
#endif
#include "../external/json/stringbuffer.h"

#include <sstream>

using namespace cocos2d;

namespace Floreto
{
	static cocos2d::Value getValueFromJson(const rapidjson::Value &jsonValue)
	{
		cocos2d::Value value;
		switch (jsonValue.GetType())
		{
			case rapidjson::kNumberType:
			{
				if (jsonValue.IsInt())
					value = jsonValue.GetInt();
				else
					value = jsonValue.GetDouble();
				break;
			}
			case rapidjson::kStringType:
			{
				value = jsonValue.GetString();
				break;
			}
			case rapidjson::kArrayType:
			{
				cocos2d::ValueVector vector;
				for (int i = 0, c = jsonValue.Size(); i < c; ++i)
					vector.push_back(getValueFromJson(jsonValue[i]));
				value = vector;
				break;
			}
			case rapidjson::kObjectType:
			{
				cocos2d::ValueMap map;
				for (auto it = jsonValue.MemberBegin(), itEnd = jsonValue.MemberEnd(); it != itEnd; ++it)
					map[it->name.GetString()] = getValueFromJson(it->value);
				value = map;
				break;
			}
			case rapidjson::kTrueType:
			{
				value = true;
				break;
			}
			case rapidjson::kFalseType:
			{
				value = false;
				break;
			}
			case rapidjson::kNullType:
			{
				value = cocos2d::Value::Null;
				break;
			}
		}

		return value;
	}

	static void getJsonFromValue(const cocos2d::Value &value, rapidjson::Value &jsonValue, rapidjson::Document::AllocatorType &allocator)
	{
		switch (value.getType())
		{
			case Value::Type::INTEGER:
			{
				jsonValue.SetInt(value.asInt());
				break;
			}
			case Value::Type::FLOAT:
			{
				jsonValue.SetDouble(value.asFloat());
				break;
			}
			case Value::Type::DOUBLE:
			{
				jsonValue.SetDouble(value.asDouble());
				break;
			}
			case Value::Type::STRING:
			{
				jsonValue.SetString(value.asString().c_str(), value.asString().length(), allocator);
				break;
			}
			case Value::Type::BOOLEAN:
			{
				jsonValue.SetBool(value.asBool());
				break;
			}
			case Value::Type::VECTOR:
			{
				const auto &vector = value.asValueVector();
				jsonValue.SetArray();
				
				for (auto it = vector.begin(), itEnd = vector.end(); it != itEnd; ++it)
				{
					rapidjson::Value v;
					getJsonFromValue(*it, v, allocator);

					jsonValue.PushBack(v, allocator);
				}
				break;
			}
			case Value::Type::MAP:
			{
				const auto &map = value.asValueMap();
				jsonValue.SetObject();
				
				for (auto it = map.begin(), itEnd = map.end(); it != itEnd; ++it)
				{
					rapidjson::Value k(it->first.c_str(), it->first.length());
					rapidjson::Value v;
					getJsonFromValue(it->second, v, allocator);

					jsonValue.AddMember(k, v, allocator);
				}
				break;
			}
			default:
			{
				jsonValue.SetNull();
				break;
			}
		}
	}

	Value JsonUtils::getValueFromJson(const char *json)
	{
		rapidjson::Document doc;
		doc.Parse<0>(json);
		if (!doc.HasParseError())
			return Floreto::getValueFromJson(doc);
		else
			return cocos2d::Value::Null;
	};
	
	char * JsonUtils::getJsonFromValue(const Value &value, bool pretty)
	{
		rapidjson::Document doc;
		Floreto::getJsonFromValue(value, doc, doc.GetAllocator());

		rapidjson::StringBuffer buffer;
		if (pretty)
		{
			rapidjson::PrettyWriter<decltype(buffer)> writer(buffer);
			writer.SetIndent(' ', 2);
			doc.Accept(writer);
		}
		else
		{
			rapidjson::Writer<decltype(buffer)> writer(buffer);
			doc.Accept(writer);
		}

		char *s = new char[buffer.GetSize() + 1];
		memcpy(s, buffer.GetString(), buffer.GetSize());
		s[buffer.GetSize()] = '\0';

		return s;
	}

#if RAPIDJSON_UNQUOTE_SUPPORTED
	char * JsonUtils::getCompactJsonFromValue(const cocos2d::Value &value, bool pretty)
	{
		rapidjson::Document doc;
		Floreto::getJsonFromValue(value, doc, doc.GetAllocator());

		rapidjson::StringBuffer buffer;
		if (pretty)
		{
			rapidjson::PrettyCompactWriter<decltype(buffer)> writer(buffer);
			writer.SetIndent(' ', 2);
			doc.Accept(writer);
		}
		else
		{
			rapidjson::CompactWriter<decltype(buffer)> writer(buffer);
			doc.Accept(writer);
		}

		char *s = new char[buffer.GetSize() + 1];
		memcpy(s, buffer.GetString(), buffer.GetSize());
		s[buffer.GetSize()] = '\0';

		return s;
	}
#endif
}
