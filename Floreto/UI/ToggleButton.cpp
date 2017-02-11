#include "ToggleButton.h"
#include "../Utils/ClassModel.h"
#include "../Utils/ValueMapUtils.h"

using namespace cocos2d;

namespace Floreto
{

	ToggleButton * ToggleButton::createWithFile(const char *onFile, const char *offFile, const ToggleDelegate &delegate, bool multitouch)
	{
		SpriteFrame *onFrame = nullptr, *offFrame = nullptr;

		auto texOn = Director::getInstance()->getTextureCache()->addImage(onFile);
		auto texOff = Director::getInstance()->getTextureCache()->addImage(offFile);

		if (texOn)
			onFrame = SpriteFrame::createWithTexture(texOn, Rect(0, 0, texOn->getContentSize().width, texOn->getContentSize().height));
		if (texOff)
			offFrame = SpriteFrame::createWithTexture(texOff, Rect(0, 0, texOff->getContentSize().width, texOff->getContentSize().height));

		return ToggleButton::createWithSpriteFrame(onFrame, offFrame, delegate, multitouch);
	}

	ToggleButton * ToggleButton::createWithSpriteFrameName(const char *onSpriteFrameName, const char *offSpriteFrameName, const ToggleDelegate &delegate, bool multitouch)
	{
		auto onFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(onSpriteFrameName);
		auto offFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(offSpriteFrameName);

		return ToggleButton::createWithSpriteFrame(onFrame, offFrame, delegate, multitouch);
	}

	ToggleButton * ToggleButton::createWithSpriteFrame(cocos2d::SpriteFrame *onSpriteFrame, cocos2d::SpriteFrame *offSpriteFrame, const ToggleDelegate &delegate, bool multitouch)
	{
		auto button = new ToggleButton();
		if (button && button->init(onSpriteFrame, offSpriteFrame, delegate, multitouch))
		{
			button->autorelease();
			return button;
		}

		CC_SAFE_DELETE(button);
		return nullptr;
	}

	ToggleButton::ToggleButton(void)
		: toggleDelegate(nullptr), toggleState(false),
		onFrame(nullptr), offFrame(nullptr)
	{
		CC_SAFE_RELEASE_NULL(onFrame);
		CC_SAFE_RELEASE_NULL(offFrame);
	}

	ToggleButton::~ToggleButton(void)
	{
	}

	bool ToggleButton::init(cocos2d::SpriteFrame *onFrame, cocos2d::SpriteFrame *offFrame, const ToggleDelegate &delegate, bool multitouch)
	{
		if (!onFrame || !offFrame)
			return false;

			if (!Button::init(offFrame, nullptr, nullptr, multitouch, false))
				return false;

		if (multitouch)
		{
			MultiTouchDelegate touchDelegate = [this](Button *button, const std::vector<Touch *> &touches, TouchPhase phase)
			{
				switch (phase)
				{
					case Floreto::TouchPhase::Ended:
					case Floreto::TouchPhase::Cancelled:
					{
						for (auto it = touches.begin(), itEnd = touches.end(); it != itEnd; ++it)
						{
							if (this->isInsideBounds(*it))
							{
								this->toggle();
								if (this->toggleDelegate)
									this->toggleDelegate(this, this->toggleState);

								break;
							}
						}
						break;
					}
					default:
					{
						break;
					}
				}
			};

			this->Button::setMultiTouchDelegate(touchDelegate, true);
		}
		else
		{
			SingleTouchDelegate touchDelegate = [this](Button *button, Touch *touch, TouchPhase phase)
			{
				switch (phase)
				{
					case Floreto::TouchPhase::Ended:
					case Floreto::TouchPhase::Cancelled:
					{
						if (this->isInsideBounds(touch))
						{
							this->toggle();
							if (this->toggleDelegate)
								this->toggleDelegate(this, this->toggleState);

							break;
						}
						break;
					}
					default:
					{
						break;
					}
				}

				return true;
			};

			this->Button::setSingleTouchDelegate(touchDelegate, true);
		}

		this->onFrame = onFrame;
		this->onFrame->retain();
		this->offFrame = offFrame;
		this->offFrame->retain();

		return true;
	}

	void ToggleButton::setOn(bool value)
	{
		if (this->toggleState == value)
			return;

		this->toggleState = value;
		this->setSpriteFrame(value ? this->onFrame : this->offFrame);
	}

	void ToggleButton::toggle(void)
	{
		this->setOn(!this->toggleState);
	}

	Node * ToggleButton::InflaterDelegate::createFromData(UIInflater *inflater, const ValueMap &data)
	{
		ValueMap::const_iterator it;
		std::string fileName1, fileName2, frameName1, frameName2;
		bool multitouch = false;

		if ((it = data.find("fileNameOn")) != data.end())
		{
			fileName1 = it->second.asString();
			fileName2 = ValueMapUtils::tryGetString("fileNameOff", data);
		}
		else if ((it = data.find("frameNameOn")) != data.end())
		{
			frameName1 = it->second.asString();
			frameName2 = ValueMapUtils::tryGetString("frameNameOff", data);
		}
		else
		{
			return nullptr;
		}

		multitouch = ValueMapUtils::tryGetBool("multitouch", data);

		ToggleButton *button = nullptr;
		if (fileName1.length() || fileName2.length())
			button = ToggleButton::createWithFile(fileName1.c_str(), fileName2.c_str(), nullptr, multitouch);
		else if (frameName1.length() || frameName2.length())
			button = ToggleButton::createWithSpriteFrameName(frameName1.c_str(), frameName2.c_str(), nullptr, multitouch);
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
