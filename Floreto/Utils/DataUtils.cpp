#include "DataUtils.h"
#include "JsonUtils.h"
#include <errno.h>

#if CC_TARGET_PLATFORM == CC_PLATFORM_WIN32
#include <WinSock2.h>
#else
#include <netinet/in.h>
#endif

using namespace cocos2d;

namespace Floreto
{
	bool DataUtils::UsingXor = false;

	Value DataUtils::readFromFile(const char *filename)
	{
		std::string data;

		std::string path = StringUtils::format("%s%s", FileUtils::getInstance()->getWritablePath().c_str(), filename);

		if (FileUtils::getInstance()->isFileExist(path))
			data = FileUtils::getInstance()->getStringFromFile(path);
		else
			data = FileUtils::getInstance()->getStringFromFile(filename);

		Value obj = JsonUtils::getValueFromJson(data.c_str());
		return obj;
	}

	DataUtils::FileWriteResult DataUtils::writeToFile(const char *filename, const Value &dataObject, bool unquoted)
	{
		auto path = StringUtils::format("%s%s", FileUtils::getInstance()->getWritablePath().c_str(), filename);

#if RAPIDJSON_UNQUOTE_SUPPORTED
		auto data = unquoted
			? JsonUtils::getCompactJsonFromValue(dataObject, true /*pretty*/)
			: JsonUtils::getJsonFromValue(dataObject, true /*pretty*/);
#else
		auto data = JsonUtils::getJsonFromValue(dataObject, true /*pretty*/);
#endif

		FILE *fp = fopen(path.c_str(), "wb");
		if (!fp)
			return FileWriteResult::FopenFailed;

		size_t dataSize = strlen(data);
		size_t result = fwrite(data, 1, dataSize, fp);
		fclose(fp);

		return result == dataSize ? FileWriteResult::OK : FileWriteResult::FwriteFailed;
	}

	Value DataUtils::readFromFileEncrypted(const char *key, const char *filename)
	{
		Data data;

		std::string path = StringUtils::format("%s%s", FileUtils::getInstance()->getWritablePath().c_str(), filename);

		if (FileUtils::getInstance()->isFileExist(path))
			data = FileUtils::getInstance()->getDataFromFile(path);
		else
			data = FileUtils::getInstance()->getDataFromFile(filename);

		if (data.isNull())
			return Value::Null;

		size_t len = (size_t)data.getSize() + 1;
		char *buffer = new char[len];

		memcpy(buffer, data.getBytes(), len - 1);
		buffer[len - 1] = 0;

		uint32_t fileChecksum = *(uint32_t *)buffer;
		fileChecksum = (uint32_t)ntohl((u_long)fileChecksum);

		if (DataUtils::UsingXor)
			DataUtils::xorString(key, buffer + sizeof(uint32_t), len - sizeof(uint32_t) - 1);
		else
			DataUtils::decodeString(key, buffer + sizeof(uint32_t), len - sizeof(uint32_t) - 1);

		uint32_t realChecksum = DataUtils::calculateChecksum(buffer + sizeof(uint32_t), len - sizeof(uint32_t) - 1);

		if (fileChecksum != realChecksum)
		{
			delete[]buffer;
			return Value::Null;
		}

		Value obj = JsonUtils::getValueFromJson(buffer + sizeof(uint32_t));
		delete[]buffer;

		return obj;
	}

	DataUtils::FileWriteResult DataUtils::writeToFileEncrypted(const char *key, const char *filename, const Value &dataObject, bool unquoted)
	{
		auto path = StringUtils::format("%s%s", FileUtils::getInstance()->getWritablePath().c_str(), filename);

		FILE *fp = fopen(path.c_str(), "wb");
		if (!fp)
			return FileWriteResult::FopenFailed;

#if RAPIDJSON_UNQUOTE_SUPPORTED
		auto str = unquoted
			? JsonUtils::getCompactJsonFromValue(dataObject, false)
			: JsonUtils::getJsonFromValue(dataObject);
#else
		auto str = JsonUtils::getJsonFromValue(dataObject, false /*pretty*/);
#endif

		size_t len = strlen(str) + sizeof(uint32_t);
		char *buffer = new char[len];

		memcpy(buffer + sizeof(int32_t), str, len - sizeof(uint32_t));
		if (DataUtils::UsingXor)
			DataUtils::xorString(key, buffer + sizeof(uint32_t), len - sizeof(uint32_t));
		else
			DataUtils::encodeString(key, buffer + sizeof(uint32_t), len - sizeof(uint32_t));

		uint32_t checksum = DataUtils::calculateChecksum(str, len - sizeof(uint32_t));
		checksum = (uint32_t)htonl((u_long)checksum);
		memcpy(buffer, (void *)&checksum, sizeof(uint32_t));

		size_t result = fwrite(buffer, 1, len, fp);
		fclose(fp);

		delete[]str;
		delete[]buffer;

		return result == len ? FileWriteResult::OK : FileWriteResult::FwriteFailed;
	}

	uint32_t DataUtils::calculateChecksum(const char *str, int32_t len)
	{
		if (len < 0)
			len = strlen(str);

		uint32_t h = 0;

		for (int32_t i = 0; i < len; ++i)
			h = ((h << 4) | (h >> 28)) ^ str[i]; // _rotl(h, 4) ^ *s

		return h;
	}

	void DataUtils::encodeString(const char *key, char *buffer, int32_t length)
	{
		if (length < 0)
			length = strlen(buffer);

		const int32_t cryptoSize = strlen(key);

		for (int32_t i = 0, k = 0; k < length; i += cryptoSize)
			for (int32_t j = 0; j < cryptoSize && k < length; ++j, ++k)
				buffer[k] = (char)((int)buffer[k] + (int)key[j]);
	}

	void DataUtils::decodeString(const char *key, char *buffer, int32_t length)
	{
		if (length < 0)
			length = strlen(buffer);

		const int32_t cryptoSize = strlen(key);

		for (int32_t i = 0, k = 0; k < length; i += cryptoSize)
			for (int32_t j = 0; j < cryptoSize && k < length; ++j, ++k)
				buffer[k] = (char)((int)buffer[k] - (int)key[j]);
	}

	void DataUtils::xorString(const char *key, char *buffer, int32_t length)
	{
		if (length < 0)
			length = strlen(buffer);

		const int32_t cryptoSize = strlen(key);

		for (int32_t i = 0, k = 0; k < length; i += cryptoSize)
			for (int32_t j = 0; j < cryptoSize && k < length; ++j, ++k)
				buffer[k] = (char)((int)buffer[k] ^ (int)key[j]);
	}

}
