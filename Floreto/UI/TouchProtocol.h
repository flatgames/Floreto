#ifndef __FLORETO_TOUCHPROTOCOL_H__
#define __FLORETO_TOUCHPROTOCOL_H__

#include "../Common.h"

#include <string>
#include <vector>

namespace Floreto
{

	class TouchGroup;

	class TouchProtocol
	{
	public:
		virtual bool isTouchEnabled(void) const = 0;
		virtual void setTouchEnabled(bool enabled) = 0;
	};

	class TouchGroup : public TouchProtocol
	{
	public:
		TouchGroup(void)
			: enabled(false), parent(nullptr)
		{
		}

		TouchGroup(const std::string &name)
			: enabled(false), name(name), parent(nullptr)
		{
		}

		virtual ~TouchGroup(void)
		{
		}

	public:
		const std::string & getName(void) const
		{
			return this->name;
		}

		void setName(const std::string &name)
		{
			this->name = name;
		}

		void addElement(TouchProtocol *element)
		{
			this->elements.push_back(element);
		}

		void removeElement(TouchProtocol *element)
		{
			for (auto it = this->elements.begin(), itEnd = this->elements.end(); it != itEnd; ++it)
			{
				if (*it == element)
				{
					this->elements.erase(it);
					break;
				}
			}
		}

		void acceptElements(const TouchGroup *group)
		{
			const auto &elements = group->elements;
			this->elements.insert(this->elements.end(), elements.begin(), elements.end());
		}

		virtual bool isTouchEnabled(void) const override
		{
			if (this->parent)
				return this->enabled && this->parent->isTouchEnabled();
			else
				return this->enabled;
		}

		virtual void setTouchEnabled(bool enabled) override
		{
			this->enabled = enabled;

			for (auto it = this->elements.begin(), itEnd = this->elements.end(); it != itEnd; ++it)
				(*it)->setTouchEnabled(enabled);
		}

	protected:
		bool enabled;
		std::string name;
		std::vector<TouchProtocol *> elements;
		
		TouchGroup *parent;
	};

}

#endif//__FLORETO_TOUCHPROTOCOL_H__
