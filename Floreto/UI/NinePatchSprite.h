#ifndef __FLORETO_NINEPATCHSPRITE_H__
#define __FLORETO_NINEPATCHSPRITE_H__

#include "../Common.h"

namespace Floreto
{

	class NinePatchSprite : public cocos2d::Node
	{
	public:
		static NinePatchSprite * createWithFile(const std::string &filename, const cocos2d::Rect &insets);
		static NinePatchSprite * createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, const cocos2d::Rect &insets);
		static NinePatchSprite * createWithSpriteFrameName(const std::string &spriteFrameName, const cocos2d::Rect &insets);

	public:
		NinePatchSprite(void);
		virtual ~NinePatchSprite(void);

		virtual void setContentSize(const cocos2d::Size &size) override;

	protected:
		virtual bool initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, const cocos2d::Rect &insets);

	protected:
		cocos2d::Rect insets;
		float leftPadding, rightPadding, topPadding, bottomPadding;

		cocos2d::Sprite *topLeft;
		cocos2d::Sprite *topCenter;
		cocos2d::Sprite *topRight;
		cocos2d::Sprite *middleLeft;
		cocos2d::Sprite *middleCenter;
		cocos2d::Sprite *middleRight;
		cocos2d::Sprite *bottomLeft;
		cocos2d::Sprite *bottomCenter;
		cocos2d::Sprite *bottomRight;
	};

}

#endif//__FLORETO_NINEPATCHSPRITE_H__
