#ifndef __FLORETO_DATAUTILS_H__
#define __FLORETO_DATAUTILS_H__

#include <stdint.h>

#include "../Common.h"

namespace Floreto
{

	class DataUtils
	{
	public:
		enum class FileWriteResult
		{
			OK,
			FopenFailed,
			FwriteFailed
		};

	public:
		static cocos2d::Value readFromFile(const char *filename);
		static FileWriteResult writeToFile(const char *filename, const cocos2d::Value &dataObject, bool unquoted = false);

		static FileWriteResult writeToFile(const char *filename, const cocos2d::ValueMap &dataObject, bool unquoted = false)
		{
			return writeToFile(filename, cocos2d::Value(dataObject), unquoted);
		}

	public:
		static cocos2d::Value readFromFileEncrypted(const char *key, const char *filename);
		static FileWriteResult writeToFileEncrypted(const char *key, const char *filename, const cocos2d::Value &dataObject, bool unquoted = false);

		static FileWriteResult writeToFileEncrypted(const char *key, const char *filename, const cocos2d::ValueMap &dataObject, bool unquoted = false)
		{
			return writeToFileEncrypted(key, filename, cocos2d::Value(dataObject), unquoted);
		}

	protected:
		static uint32_t calculateChecksum(const char *str, int32_t len = -1);

		static void encodeString(const char *key, char *buffer, int32_t length = -1);
		static void decodeString(const char *key, char *buffer, int32_t length = -1);

		static void xorString(const char *key, char *buffer, int32_t length = -1);

	public:
		// If this flag is true, data is encrypted using XOR algorithm
		// instead of add/subtract algorithm.
		// Default value is false.
		static bool UsingXor;
	};

}

#endif//__FLORETO_DATAUTILS_H__
