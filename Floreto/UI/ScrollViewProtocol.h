#ifndef __FLORETO_SCROLLVIEWPROTOCOL_H__
#define __FLORETO_SCROLLVIEWPROTOCOL_H__

#include "../Common.h"
#include "TouchPhase.h"

namespace Floreto
{

	class ScrollViewProtocol
	{
	public:
		enum class Orientation
		{
			Horizontal,
			Vertical
		};

	public:
		virtual void takeTouch(cocos2d::Touch *touch, TouchPhase phase) = 0;
		virtual Orientation getOrientation(void) = 0;
		virtual float getScrollThreshold(void) = 0;

		virtual bool isInsideView(cocos2d::Touch *touch) = 0;
	};

	class ScrollViewAwareProtocol
	{
	public:
		ScrollViewAwareProtocol(void)
			: scrollView(nullptr), ignoringScrollView(false)
		{
		}

		inline virtual ScrollViewProtocol * getScrollView(void)
		{
			return this->scrollView;
		}

		inline virtual void setScrollView(ScrollViewProtocol *scrollView)
		{
			this->scrollView = scrollView;
		}

		inline virtual bool isIgnoringScrollView(void) const
		{
			return this->ignoringScrollView;
		}

		inline virtual void setIgnoringScrollView(bool ignoring)
		{
			this->ignoringScrollView = ignoring;
		}

	public:
		ScrollViewProtocol *scrollView;
		bool ignoringScrollView;
	};

}

#endif//__FLORETO_SCROLLVIEWPROTOCOL_H__
