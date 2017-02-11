#ifndef __FLORETO_UICONTAINERPROTOCOL_H__
#define __FLORETO_UICONTAINERPROTOCOL_H__

#include "../Common.h"
#include "UIElementProtocol.h"

#include <unordered_map>
#include <string>

namespace Floreto
{
	class UIContainerProtocol
	{
	protected:
		std::unordered_map<std::string, UIElementProtocol *> elements;

	public:
		bool addElement(const char *name, UIElementProtocol *element)
		{
			if (!name || !name[0])
			{
				return false;
			}
			else
			{
				this->addElement(std::string(name), element); return true;
			}
		}

		bool addElement(const std::string &name, UIElementProtocol *element)
		{
			if (!name[0])
				return false;

			auto it = this->elements.find(name);
			if (it != this->elements.end())
				return false;

			this->elements[name] = element;
			return true;
		}

		UIElementProtocol * getElementByName(const char *name)
		{
			return this->getElementByName(std::string(name));
		}

		UIElementProtocol * getElementByName(const std::string &name)
		{
			auto it = this->elements.find(name);
			if (it != this->elements.end())
				return it->second;
			else
				return nullptr;
		}
		
		template<typename T>
		T * getElementByName(const char *name)
		{
			return this->getElementByName<T>(std::string(name));
		}

		template<typename T>
		T * getElementByName(const std::string &name)
		{
			return static_cast<T *>(this->getElementByName(name));
		}
	};
}

#endif//__FLORETO_UICONTAINERPROTOCOL_H__
