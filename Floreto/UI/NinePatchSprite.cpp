#include "NinePatchSprite.h"

using namespace cocos2d;

namespace Floreto
{
	NinePatchSprite * NinePatchSprite::createWithFile(const std::string &filename, const cocos2d::Rect &insets)
	{
		auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
		if (!texture)
			return nullptr;

		auto size = texture->getContentSize();
		auto spriteFrame = SpriteFrame::createWithTexture(texture, Rect(0, 0, size.width, size.height));

		return NinePatchSprite::createWithSpriteFrame(spriteFrame, insets);
	}

	NinePatchSprite * NinePatchSprite::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, const cocos2d::Rect &insets)
	{
		auto sprite = new NinePatchSprite();
		if (sprite && sprite->initWithSpriteFrame(spriteFrame, insets))
		{
			sprite->autorelease();
			return sprite;
		}

		CC_SAFE_DELETE(sprite);
		return nullptr;
	}

	NinePatchSprite * NinePatchSprite::createWithSpriteFrameName(const std::string &spriteFrameName, const cocos2d::Rect &insets)
	{
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
		return NinePatchSprite::createWithSpriteFrame(spriteFrame, insets);
	}

	NinePatchSprite::NinePatchSprite(void)
		: topLeft(nullptr), topCenter(nullptr), topRight(nullptr),
		middleLeft(nullptr), middleCenter(nullptr), middleRight(nullptr),
		bottomLeft(nullptr), bottomCenter(nullptr), bottomRight(nullptr)
	{
	}

	NinePatchSprite::~NinePatchSprite(void)
	{
	}

	void NinePatchSprite::setContentSize(const cocos2d::Size &size)
	{
		Node::setContentSize(size);

		float xScale = (size.width - leftPadding - rightPadding) / insets.size.width;
		float yScale = (size.height - topPadding - bottomPadding) / insets.size.height;

		this->topLeft->setPosition(0, size.height);
		this->topCenter->setPosition(size.width / 2, size.height);
		this->topCenter->setScaleX(xScale);
		this->topRight->setPosition(size.width, size.height);

		this->middleLeft->setPosition(0, size.height / 2);
		this->middleLeft->setScaleY(yScale);
		this->middleCenter->setPosition(size.width / 2, size.height / 2);
		this->middleCenter->setScale(xScale, yScale);
		this->middleRight->setPosition(size.width, size.height / 2);
		this->middleRight->setScaleY(yScale);

		this->bottomLeft->setPosition(0, 0);
		this->bottomCenter->setPosition(size.width / 2, 0);
		this->bottomCenter->setScaleX(xScale);
		this->bottomRight->setPosition(size.width, 0);
	}

	bool NinePatchSprite::initWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, const cocos2d::Rect &insets)
	{
		CCASSERT(spriteFrame != nullptr, "ArgumentNullException");

		auto texture = spriteFrame->getTexture();
		auto rect = spriteFrame->getRect();

		float leftPadding = insets.origin.x,
			rightPadding = rect.size.width - insets.size.width - insets.origin.x,
			topPadding = rect.size.height - insets.size.height - insets.origin.y,
			bottomPadding = insets.origin.y;

		Rect frameRect;

		// top left
		frameRect.origin.setPoint(rect.origin.x, rect.origin.y);
		frameRect.size.setSize(leftPadding, topPadding);
		this->topLeft = Sprite::createWithTexture(texture, frameRect);
		this->topLeft->setAnchorPoint(Vec2::ANCHOR_TOP_LEFT);

		// top center
		frameRect.origin.setPoint(rect.origin.x + leftPadding, rect.origin.y);
		frameRect.size.setSize(insets.size.width, topPadding);
		this->topCenter = Sprite::createWithTexture(texture, frameRect);
		this->topCenter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_TOP);

		// top right
		frameRect.origin.setPoint(rect.origin.x + leftPadding + insets.size.width, rect.origin.y);
		frameRect.size.setSize(rightPadding, topPadding);
		this->topRight = Sprite::createWithTexture(texture, frameRect);
		this->topRight->setAnchorPoint(Vec2::ANCHOR_TOP_RIGHT);

		// middle left
		frameRect.origin.setPoint(rect.origin.x, rect.origin.y + topPadding);
		frameRect.size.setSize(leftPadding, insets.size.height);
		this->middleLeft = Sprite::createWithTexture(texture, frameRect);
		this->middleLeft->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);

		// middle center
		frameRect.origin.setPoint(rect.origin.x + leftPadding, rect.origin.y + topPadding);
		frameRect.size.setSize(insets.size.width, insets.size.height);
		this->middleCenter = Sprite::createWithTexture(texture, frameRect);
		this->middleCenter->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		// middle right
		frameRect.origin.setPoint(rect.origin.x + leftPadding + insets.size.width, rect.origin.y + topPadding);
		frameRect.size.setSize(rightPadding, insets.size.height);
		this->middleRight = Sprite::createWithTexture(texture, frameRect);
		this->middleRight->setAnchorPoint(Vec2::ANCHOR_MIDDLE_RIGHT);

		// bottom left
		frameRect.origin.setPoint(rect.origin.x, rect.origin.y + topPadding + insets.size.height);
		frameRect.size.setSize(leftPadding, bottomPadding);
		this->bottomLeft = Sprite::createWithTexture(texture, frameRect);
		this->bottomLeft->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);

		// bottom center
		frameRect.origin.setPoint(rect.origin.x + leftPadding, rect.origin.y + topPadding + insets.size.height);
		frameRect.size.setSize(insets.size.width, bottomPadding);
		this->bottomCenter = Sprite::createWithTexture(texture, frameRect);
		this->bottomCenter->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);

		// bottom right
		frameRect.origin.setPoint(rect.origin.x + leftPadding + insets.size.width, rect.origin.y + topPadding + insets.size.height);
		frameRect.size.setSize(rightPadding, bottomPadding);
		this->bottomRight = Sprite::createWithTexture(texture, frameRect);
		this->bottomRight->setAnchorPoint(Vec2::ANCHOR_BOTTOM_RIGHT);

		this->insets = insets;

		this->leftPadding = leftPadding;
		this->rightPadding = rightPadding;
		this->topPadding = topPadding;
		this->bottomPadding = bottomPadding;

		this->addChild(topLeft, 0);
		this->addChild(topCenter, 0);
		this->addChild(topRight, 0);
		this->addChild(middleLeft, 0);
		this->addChild(middleCenter, 0);
		this->addChild(middleRight, 0);
		this->addChild(bottomLeft, 0);
		this->addChild(bottomCenter, 0);
		this->addChild(bottomRight, 0);

		this->setContentSize(rect.size);
		this->setCascadeColorEnabled(true);
		this->setCascadeOpacityEnabled(true);
		this->ignoreAnchorPointForPosition(false);
		this->setAnchorPoint(Vec2::ANCHOR_MIDDLE);

		return true;
	}

}
