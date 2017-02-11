#include "VisibleView.h"

namespace Floreto
{
	float VisibleView::_width = 0;
	float VisibleView::_height = 0;
	float VisibleView::_x = 0;
	float VisibleView::_y = 0;
	float VisibleView::_centerX = 0;
	float VisibleView::_centerY = 0;
	float VisibleView::_onePixel = 1;

	void VisibleView::init(float designWidth, float designHeight, Policy policy)
	{
		using namespace cocos2d;

		auto glview = Director::getInstance()->getOpenGLView();
		auto frameSize = Director::getInstance()->getVisibleSize();

		switch (policy)
		{
			case Policy::FixedWidth:
			{
				if (frameSize.width / designWidth < frameSize.height / designHeight)
				{
					// Screen height is bigger than design height.
					glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::FIXED_WIDTH);
				}
				else
				{
					// Screen height may be smaller than design height.
					glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::NO_BORDER);
				}
				break;
			}
			case Policy::FixedHeight:
			{
				if (frameSize.height / designHeight < frameSize.width / designWidth)
				{
					// Screen width is bigger than design width.
					glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::FIXED_HEIGHT);
				}
				else
				{
					// Screen width may be smaller than design width.
					glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::NO_BORDER);
				}
				break;
			}
			case Policy::NoBorder:
			{
				glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::NO_BORDER);
				break;
			}
			case Policy::ShowAll:
			{
				glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::SHOW_ALL);
				break;
			}
			case Policy::ShowAllExtended:
			{
				if (frameSize.width / designWidth > frameSize.height / designHeight)
				{
					// Screen width is bigger than design width.
					glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::FIXED_HEIGHT);
				}
				else
				{
					// Screen height may be bigger than design height.
					glview->setDesignResolutionSize(designWidth, designHeight, ResolutionPolicy::FIXED_WIDTH);
				}
				break;
			}
		}

		auto designSize = glview->getDesignResolutionSize();
		_width = designSize.width;
		_height = designSize.height;
		
		auto visibleOrigin = Director::getInstance()->getVisibleOrigin();
		_x = visibleOrigin.x;
		_y = visibleOrigin.y;
		_centerX = _width / 2;
		_centerY = _height / 2;
		_onePixel = designSize.width / frameSize.width;

		_width -= _x * 2;
		_height -= _y * 2;
	}
}
