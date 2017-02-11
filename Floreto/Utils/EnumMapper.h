#ifndef __FLORETO_ENUMMAPPER_H__
#define __FLORETO_ENUMMAPPER_H__

#include <cocos2d.h>

namespace Floreto
{

	// ONE-PLACE PRINCIPLE: Place the definitions of the enum members and their corresponding strings in one place.

	// A container type that maps every enum member value to its corresponding name string and vice versa.
	template<typename T>
	class EnumMapper : public cocos2d::Ref
	{
	protected:
		std::map<T, std::string> _valueToName;
		std::map<std::string, T> _nameToValue;

	public:
		static EnumMapper * createWithNames(const char **names, size_t nameCount)
		{
			auto mapper = new EnumMapper<T>();
			if (mapper && mapper->initWithNames(names, nameCount))
			{
				mapper->autorelease();
				return mapper;
			}

			CC_SAFE_DELETE(mapper);
			return nullptr;
		}

		static EnumMapper * createWithValues(T *values, const char **names, size_t nameCount)
		{
			auto mapper = new EnumMapper<T>();
			if (mapper && mapper->initWithValues(values, names, nameCount))
			{
				mapper->autorelease();
				return mapper;
			}

			CC_SAFE_DELETE(mapper);
			return nullptr;
		}

	protected:
		bool initWithNames(const char **names, size_t nameCount)
		{
			if (names)
				this->addValues(names, nameCount);

			return true;
		}

		bool initWithValues(T *values, const char **names, size_t nameCount)
		{
			if (names)
				this->addValues(values, names, nameCount);

			return true;
		}

	public:
		EnumMapper(void)
		{
		}

		virtual ~EnumMapper(void)
		{
		}

	public:
		void addValue(T value, const char *name)
		{
			this->_valueToName[value] = name;
			this->_nameToValue[name] = value;
		}

		void addValues(const char **names, size_t nameCount)
		{
			for (size_t i = 0; i < nameCount; ++i)
			{
				this->_valueToName[(T)i] = names[i];
				this->_nameToValue[names[i]] = (T)i;
			}
		}

		void addValues(T *values, const char **names, size_t nameCount)
		{
			for (size_t i = 0; i < nameCount; ++i)
			{
				this->_valueToName[values[i]] = names[i];
				this->_nameToValue[names[i]] = values[i];
			}
		}

		T getValueByName(const std::string &name) const
		{
			auto it = this->_nameToValue.find(name);
			if (it != this->_nameToValue.end())
				return it->second;
			else
				return T();
		}

		const std::string & getNameByValue(T value) const
		{
			static const std::string Empty = "";

			auto it = this->_valueToName.find(value);
			if (it != this->_valueToName.end())
				return it->second;
			else
				return Empty;
		}
	};

	template<typename T>
	class EnumMapperAdapter
	{
	protected:
		EnumMapper<T> _mapper;

	public:
		virtual ~EnumMapperAdapter(void)
		{
		}

	protected:
		void initMapper(const char **names, size_t nameCount)
		{
			_mapper.addValues(names, nameCount);
		}

		void initMapper(T *values, const char **names, size_t nameCount)
		{
			_mapper.addValues(values, names, nameCount);
		}

	public:
		virtual T getValueByName(const std::string &name) const
		{
			return _mapper.getValueByName(name);
		}

		virtual const std::string & getNameByValue(T value) const
		{
			return _mapper.getNameByValue(value);
		}
	};

}

#define InitEnumMapper(__type__) \
	do { Floreto::EnumMapperAdapter<__type__>::initMapper(EnumNames_##__type__, sizeof(EnumNames_##__type__) / sizeof(const char *)); } while (false)

#define InitEnumMapperEx(__type__, __typeName__) \
	do { Floreto::EnumMapperAdapter<__type__>::initMapper(EnumNames_##__typeName__, sizeof(EnumNames_##__typeName__) / sizeof(const char *)); } while (false)

#define InitEnumMapperWithValues(__type__) \
	do { Floreto::EnumMapperAdapter<__type__>::initMapper(EnumValues_##__type__, EnumNames_##__type__, sizeof(EnumNames_##__type__) / sizeof(const char *)); } while (false)

#define InitEnumMapperWithValuesEx(__type__, __typeName__) \
	do { Floreto::EnumMapperAdapter<__type__>::initMapper(EnumValues_##__type__, EnumNames_##__typeName__, sizeof(EnumNames_##__typeName__) / sizeof(const char *)); } while (false)

#endif//__FLORETO_ENUMMAPPER_H__
