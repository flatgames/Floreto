#ifndef __FLORETO_UIUTILS_H__
#define __FLORETO_UIUTILS_H__

#include "../Common.h"

namespace Floreto
{

	class UIUtils
	{
	public:
		static void calculateContentNodeSize(cocos2d::Node *contentNode, float xPadding = 0, float yPadding = 0, bool ignoreInvisibleChildren = false);
		static cocos2d::Sprite * createRectangle(float width, float height, cocos2d::Color3B color);
		static cocos2d::Color3B lightenColor(const cocos2d::Color3B &color, float percent);
		static cocos2d::Color3B darkenColor(const cocos2d::Color3B &color, float percent);
	};

}

#endif//__FLORETO_UIUTILS_H__
