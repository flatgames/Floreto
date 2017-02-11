#ifndef __FLORETO_UIELEMENTPROTOCOL_H__
#define __FLORETO_UIELEMENTPROTOCOL_H__

#include "../Common.h"
#include <string>

namespace Floreto
{
	class UIElementProtocol
	{
	protected:
		std::string elementName;

	public:
		virtual ~UIElementProtocol(void) { }

		const std::string & getName(void) const
		{
			return this->elementName;
		}

		void setName(const char *name)
		{
			this->elementName = name;
		}

		void setName(const std::string &name)
		{
			this->elementName = name;
		}
	};
}

#endif//__FLORETO_UIELEMENTPROTOCOL_H__
