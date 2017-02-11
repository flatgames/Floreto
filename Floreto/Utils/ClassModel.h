#ifndef __FLORETO_CLASSMODEL_H__
#define __FLORETO_CLASSMODEL_H__

#include <cocos2d.h>
#include <string>

namespace Floreto
{

	template<typename T>
	class ClassModel
	{
	public:
		virtual void serialize(const T &obj, cocos2d::ValueMap &data)
		{
		}

		virtual void deserialize(T &obj, const cocos2d::ValueMap &data)
		{
		}

		virtual T * createFromData(const cocos2d::ValueMap &data)
		{
			return nullptr;
		}
	};

}

// Friendify a specialized ClassModel for the given class and publicly declare a typedef as Model.
// Warning: everything that comes after this will be public. Explicitly use other modifiers if needed.
#define DeclareModelForClass(__class__) \
	friend class Floreto::ClassModel<__class__>; \
	public: typedef Floreto::ClassModel<__class__> Model

#endif//__FLORETO_CLASSMODEL_H__
