#include "TiledButton.h"
#include "../Utils/ClassModel.h"
#include "../Utils/ValueMapUtils.h"

using namespace cocos2d;

namespace Floreto
{
	enum
	{
		kTagAutoSwitchFrame = 5469783
	};

	static const float AUTO_SWITCH_FRAME_DELAY = 0.25f;

	TiledButton * TiledButton::createWithFile(const char *normalFile, const char *pressedFile, const SingleTouchDelegate &delegate)
	{
		SpriteFrame *normalFrame = nullptr, *pressedFrame = nullptr;

		auto texNormal = Director::getInstance()->getTextureCache()->addImage(normalFile);
		auto texPressed = Director::getInstance()->getTextureCache()->addImage(pressedFile);
		
		if (texNormal)
			normalFrame = SpriteFrame::createWithTexture(texNormal, Rect(0, 0, texNormal->getContentSize().width, texNormal->getContentSize().height));
		if (texPressed)
			pressedFrame = SpriteFrame::createWithTexture(texPressed, Rect(0, 0, texPressed->getContentSize().width, texPressed->getContentSize().height));

		return TiledButton::createWithSpriteFrame(normalFrame, pressedFrame, delegate);
	}

	TiledButton * TiledButton::createWithSpriteFrameName(const char *normalSpriteFrameName, const char *pressedSpriteFrameName, const SingleTouchDelegate &delegate)
	{
		auto normalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(normalSpriteFrameName);
		auto pressedFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pressedSpriteFrameName);
		
		return TiledButton::createWithSpriteFrame(normalFrame, pressedFrame, delegate);
	}

	TiledButton * TiledButton::createWithSpriteFrame(SpriteFrame *normalSpriteFrame, SpriteFrame *pressedSpriteFrame, const SingleTouchDelegate &delegate)
	{
		auto button = new TiledButton();
		if (button && button->init(normalSpriteFrame, pressedSpriteFrame, delegate, nullptr, false))
		{
			button->autorelease();
			return button;
		}

		CC_SAFE_DELETE(button);
		return nullptr;
	}

	TiledButton * TiledButton::createWithFile(const char *normalFile, const char *pressedFile, const MultiTouchDelegate &delegate)
	{
		SpriteFrame *normalFrame = nullptr, *pressedFrame = nullptr;

		auto texNormal = Director::getInstance()->getTextureCache()->addImage(normalFile);
		auto texPressed = Director::getInstance()->getTextureCache()->addImage(pressedFile);

		if (texNormal)
			normalFrame = SpriteFrame::createWithTexture(texNormal, Rect(0, 0, texNormal->getContentSize().width, texNormal->getContentSize().height));
		if (texPressed)
			pressedFrame = SpriteFrame::createWithTexture(texPressed, Rect(0, 0, texPressed->getContentSize().width, texPressed->getContentSize().height));

		return TiledButton::createWithSpriteFrame(normalFrame, pressedFrame, delegate);
	}

	TiledButton * TiledButton::createWithSpriteFrameName(const char *normalSpriteFrameName, const char *pressedSpriteFrameName, const MultiTouchDelegate &delegate)
	{
		auto normalFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(normalSpriteFrameName);
		auto pressedFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(pressedSpriteFrameName);

		return TiledButton::createWithSpriteFrame(normalFrame, pressedFrame, delegate);
	}

	TiledButton * TiledButton::createWithSpriteFrame(SpriteFrame *normalSpriteFrame, SpriteFrame *pressedSpriteFrame, const MultiTouchDelegate &delegate)
	{
		auto button = new TiledButton();
		if (button && button->init(normalSpriteFrame, pressedSpriteFrame, nullptr, delegate, true))
		{
			button->autorelease();
			return button;
		}

		CC_SAFE_DELETE(button);
		return nullptr;
	}

	TiledButton::TiledButton(void)
		: normalFrame(nullptr), pressedFrame(nullptr)
	{
	}

	TiledButton::~TiledButton(void)
	{
		CC_SAFE_RELEASE_NULL(normalFrame);
		CC_SAFE_RELEASE_NULL(pressedFrame);
	}

	bool TiledButton::init(SpriteFrame *normalSpriteFrame, SpriteFrame *pressedSpriteFrame, const SingleTouchDelegate &singleTouchDelegate, const MultiTouchDelegate &multiTouchDelegate, bool multitouch)
	{
		if (!Button::init(normalSpriteFrame, singleTouchDelegate, multiTouchDelegate, multitouch, false))
			return false;

		this->normalFrame = normalSpriteFrame;
		this->pressedFrame = pressedSpriteFrame;

		CC_SAFE_RETAIN(this->normalFrame);
		CC_SAFE_RETAIN(this->pressedFrame);

		return true;
	}

	void TiledButton::replaceSpriteFrame(SpriteFrame *spriteFrame)
	{
		Button::replaceSpriteFrame(spriteFrame);

		CC_SAFE_RELEASE_NULL(this->normalFrame);
		this->normalFrame = spriteFrame;
		CC_SAFE_RETAIN(this->normalFrame);
	}

	void TiledButton::onTouchEffect(TouchPhase phase, bool autoEnd)
	{
		switch (phase)
		{
			case Floreto::TouchPhase::Began:
			{
				if (this->pressedFrame)
					this->setSpriteFrame(this->pressedFrame);

				auto act = this->getActionByTag(kTagAutoSwitchFrame);
				if (act)
					this->stopAction(act);

				if (autoEnd)
				{
					act = Sequence::create(
						DelayTime::create(AUTO_SWITCH_FRAME_DELAY),
						CallFunc::create([this](void) { if (this->normalFrame) this->setSpriteFrame(this->normalFrame); }),
						nullptr);
					act->setTag(kTagAutoSwitchFrame);
					this->runAction(act);
				}

				break;
			}
			case Floreto::TouchPhase::Ended:
			case Floreto::TouchPhase::Cancelled:
			{
				auto act = this->getActionByTag(kTagAutoSwitchFrame);
				if (act)
					this->stopAction(act);

				if (this->normalFrame)
					this->setSpriteFrame(this->normalFrame);

				break;
			}
			default:
			{
				break;
			}
		}
	}

	Node * TiledButton::InflaterDelegate::createFromData(UIInflater *inflater, const ValueMap &data)
	{
		ValueMap::const_iterator it;
		std::string fileName1, fileName2, frameName1, frameName2;
		bool multitouch = false;

		if ((it = data.find("fileName1")) != data.end())
		{
			fileName1 = it->second.asString();
			fileName2 = ValueMapUtils::tryGetString("fileName2", data);
		}
		else if ((it = data.find("frameName1")) != data.end())
		{
			frameName1 = it->second.asString();
			frameName2 = ValueMapUtils::tryGetString("frameName2", data);
		}
		else
		{
			return nullptr;
		}

		multitouch = ValueMapUtils::tryGetBool("multitouch", data);

		TiledButton *button = nullptr;
		if (fileName1.length() || fileName2.length())
			button = multitouch ?
			TiledButton::createWithFile(fileName1.c_str(), fileName2.c_str(), static_cast<Button::MultiTouchDelegate>(nullptr)) :
			TiledButton::createWithFile(fileName1.c_str(), fileName2.c_str(), static_cast<Button::SingleTouchDelegate>(nullptr));
		else if (frameName1.length() || frameName2.length())
			button = multitouch ?
			TiledButton::createWithSpriteFrameName(frameName1.c_str(), frameName2.c_str(), static_cast<Button::MultiTouchDelegate>(nullptr)) :
			TiledButton::createWithSpriteFrameName(frameName1.c_str(), frameName2.c_str(), static_cast<Button::SingleTouchDelegate>(nullptr));
		else
			return nullptr;

		if (!button)
			return nullptr;

		inflater->initElementProperties(button, data);
		inflater->initNodeProperties(button, data);
		inflater->initTouchProtocolProperties(button, data);
		inflater->initScrollViewAwareness(button, data);

		return button;
	}
}
