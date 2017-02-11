#include "Button.h"
#include "../Utils/ClassModel.h"
#include "../Utils/ValueMapUtils.h"

using namespace cocos2d;

namespace Floreto
{
	enum
	{
		kTagPopoutBegan = 123,
		kTagPopoutEnded = 321
	};

	static const float POPOUT_DURATION = 0.1f;

	Button * Button::create(const SingleTouchDelegate &delegate, bool popout)
	{
		return Button::createWithSpriteFrame(static_cast<SpriteFrame *>(nullptr), delegate, popout);
	}

	Button * Button::createWithFile(const char *filename, const SingleTouchDelegate &delegate, bool popout)
	{
		SpriteFrame *spriteFrame = nullptr;
		auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
		if (texture)
			spriteFrame = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));

		return Button::createWithSpriteFrame(spriteFrame, delegate, popout);
	}

	Button * Button::createWithSpriteFrameName(const char *spriteFrameName, const SingleTouchDelegate &delegate, bool popout)
	{
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
		return Button::createWithSpriteFrame(spriteFrame, delegate, popout);
	}

	Button * Button::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, const SingleTouchDelegate &delegate, bool popout)
	{
		auto button = new Button();
		if (button && button->init(spriteFrame, delegate, nullptr, false, popout))
		{
			button->autorelease();
			return button;
		}

		CC_SAFE_DELETE(button);
		return button;
	}

	Button * Button::create(const MultiTouchDelegate &delegate, bool popout)
	{
		return Button::createWithSpriteFrame(static_cast<SpriteFrame *>(nullptr), delegate, popout);
	}

	Button * Button::createWithFile(const char *filename, const MultiTouchDelegate &delegate, bool popout)
	{
		SpriteFrame *spriteFrame = nullptr;
		auto texture = Director::getInstance()->getTextureCache()->addImage(filename);
		if (texture)
			spriteFrame = SpriteFrame::createWithTexture(texture, Rect(0, 0, texture->getContentSize().width, texture->getContentSize().height));

		return Button::createWithSpriteFrame(spriteFrame, delegate, popout);
	}

	Button * Button::createWithSpriteFrameName(const char *spriteFrameName, const MultiTouchDelegate &delegate, bool popout)
	{
		auto spriteFrame = SpriteFrameCache::getInstance()->getSpriteFrameByName(spriteFrameName);
		return Button::createWithSpriteFrame(spriteFrame, delegate, popout);
	}

	Button * Button::createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, const MultiTouchDelegate &delegate, bool popout)
	{
		auto button = new Button();
		if (button && button->init(spriteFrame, nullptr, delegate, true, popout))
		{
			button->autorelease();
			return button;
		}

		CC_SAFE_DELETE(button);
		return button;
	}

	Button::SingleTouchDelegate Button::createSimpleDelegate(const SimpleDelegate &delegate)
	{
		return MakeSingleTouchDelegate([delegate], button, touch, phase)
		{
			switch (phase)
			{
				case Floreto::TouchPhase::Began:
				{
					return true;
				}
				case Floreto::TouchPhase::Ended:
				case Floreto::TouchPhase::Cancelled:
				{
					if (button->isInsideBounds(touch))
						delegate();

					break;
				}
				default:
					break;
			}
			
			return true;
		};
	}

	bool Button::isInsideBounds(Node *node, float x, float y)
	{
		auto p = node->getParent()->convertToNodeSpace(Point(x, y));
		return node->getBoundingBox().containsPoint(p);
	}

	bool Button::isInsideBounds(cocos2d::Node *node, cocos2d::Touch *touch)
	{
		auto p = node->getParent()->convertToNodeSpace(touch->getLocation());
		return node->getBoundingBox().containsPoint(p);
	}

	bool Button::isInsideBounds(float x, float y)
	{
		return Button::isInsideBounds(this, x, y);
	}

	bool Button::isInsideBounds(cocos2d::Touch *touch)
	{
		return Button::isInsideBounds(this, touch);
	}

	Button::Button(void)
		: touchEnabled(false), multitouch(false), acceptingAllTouches(true), multiTouchId(0),
		singleTouchDelegate(nullptr), listenerSingleTouch(nullptr),
		popout(false), popoutSprite(nullptr), popoutScale(1),
		orgTouchValue(0), capturingTouchForScrollView(false), capturingMovesForScrollView(false)
	{
	}

	Button::~Button(void)
	{
		CC_SAFE_RELEASE_NULL(listenerSingleTouch);
		CC_SAFE_RELEASE_NULL(listenerMultiTouch);
	}

	bool Button::init(SpriteFrame *spriteFrame, const SingleTouchDelegate &singleTouchDelegate, const MultiTouchDelegate &multiTouchDelegate, bool multitouch, bool popout)
	{
		if ((spriteFrame && Sprite::initWithSpriteFrame(spriteFrame)) || (!spriteFrame && Sprite::init()))
		{
			this->singleTouchDelegate = singleTouchDelegate;
			this->multiTouchDelegate = multiTouchDelegate;

			this->setMultitouch(multitouch);

			this->popout = popout;
			this->replaceSpriteFrame(spriteFrame);

			return true;
		}
		else
		{
			return false;
		}
	}

	void Button::onEnter(void)
	{
		Sprite::onEnter();
	}

	void Button::onExit(void)
	{
		this->setTouchEnabled(false);
		Sprite::onExit();
	}

	void Button::cleanup(void)
	{
		this->setTouchEnabled(false);
		Sprite::cleanup();
	}

	void Button::setSingleTouchDelegate(const SingleTouchDelegate &delegate, bool forceful)
	{
		if (this->multitouch == false)
		{
			this->singleTouchDelegate = delegate;
		}
		else if (forceful)
		{
			this->singleTouchDelegate = delegate;
			this->setMultitouch(false);
		}
	}

	void Button::setMultiTouchDelegate(const MultiTouchDelegate &delegate, bool forceful)
	{
		if (this->multitouch == true)
		{
			this->multiTouchDelegate = delegate;
		}
		else if (forceful)
		{
			this->multiTouchDelegate = delegate;
			this->setMultitouch(true);
		}
	}

	void Button::setMultitouch(bool multitouch)
	{
		// If touch-move doesn't change and either listener is available
		if (this->multitouch == multitouch && (listenerSingleTouch || listenerMultiTouch))
			return;

		this->setTouchEnabled(false);
		this->multitouch = multitouch;

		if (multitouch)
		{
			CC_SAFE_RELEASE_NULL(listenerSingleTouch);
			this->initMultiTouchListener();
		}
		else
		{
			CC_SAFE_RELEASE_NULL(listenerMultiTouch);
			this->initSingleTouchListener();
		}
	}

	void Button::initSingleTouchListener(void)
	{
		this->listenerSingleTouch = EventListenerTouchOneByOne::create();
		this->listenerSingleTouch->retain();
		this->listenerSingleTouch->setSwallowTouches(true);

		this->listenerSingleTouch->onTouchBegan = [this](Touch *touch, Event *event)->bool
		{
			this->capturingTouchForScrollView = false;
			this->capturingMovesForScrollView = false;

			// If this button is aware of scrollview and the touch point is outside the view bounds => ignore this event.
			if (!this->ignoringScrollView && this->scrollView && !this->scrollView->isInsideView(touch))
				return false;

			// If the touch point is simply outside this button bounds => ignore this event.
			if (!this->isInsideBounds(touch))
				return false;

			// If a scrollview exists, mark the original touch position.
			if (this->scrollView)
			{
				if (this->scrollView->getOrientation() == ScrollViewProtocol::Orientation::Vertical)
					this->orgTouchValue = touch->getLocation().y;
				else
					this->orgTouchValue = touch->getLocation().x;
			}

			bool capturingTouch = false;

			// The callback decides whether to capture the event or not.
			if (this->singleTouchDelegate)
				capturingTouch = this->singleTouchDelegate(this, touch, TouchPhase::Began);

			if (!capturingTouch)
			{
				// We still need to capture the event for the scrollview if it exists.
				if (this->ignoringScrollView == false && this->scrollView != nullptr)
					this->capturingTouchForScrollView = true;
				else
					return false;
			}

			this->onTouchEffect(TouchPhase::Began, !capturingTouch);
			return true;
		};
		this->listenerSingleTouch->onTouchMoved = [this](Touch *touch, Event *event)
		{
			// Mark the new position for the scrollview to use.
			float newValue = 0;
			if (this->scrollView)
			{
				if (this->scrollView->getOrientation() == ScrollViewProtocol::Orientation::Vertical)
					newValue = touch->getLocation().y;
				else
					newValue = touch->getLocation().x;
			}

			bool touchSentToScrollView = false;

			if (!this->ignoringScrollView && this->scrollView)
			{
				if (this->capturingMovesForScrollView)
				{
					// Touch already captured. Now send Moved event.
					this->scrollView->takeTouch(touch, TouchPhase::Moved);
					touchSentToScrollView = true;
				}
				else if (fabsf(this->orgTouchValue - newValue) > this->scrollView->getScrollThreshold())
				{
					// Touch not captured yet. Capture it and send Began event.
					this->scrollView->takeTouch(touch, TouchPhase::Began);
					this->capturingTouchForScrollView = true;
					this->capturingMovesForScrollView = true;
					this->onTouchEffect(TouchPhase::Cancelled);
					touchSentToScrollView = true;

					if (this->singleTouchDelegate)
						this->singleTouchDelegate(this, touch, TouchPhase::Dismissed);
				}
			}

			if (!this->capturingTouchForScrollView && !touchSentToScrollView && this->singleTouchDelegate)
			{
				this->singleTouchDelegate(this, touch, TouchPhase::Moved);
				this->onTouchEffect(TouchPhase::Moved);
			}
		};
		this->listenerSingleTouch->onTouchEnded = [this](Touch *touch, Event *event)
		{
			if (this->capturingTouchForScrollView && this->scrollView)
				this->scrollView->takeTouch(touch, TouchPhase::Ended);

			if (!this->capturingTouchForScrollView && this->singleTouchDelegate)
			{
				this->singleTouchDelegate(this, touch, TouchPhase::Ended);
				this->onTouchEffect(TouchPhase::Ended);
			}

			this->capturingTouchForScrollView = false;
		};
		this->listenerSingleTouch->onTouchCancelled = [this](Touch *touch, Event *event)
		{
			if (this->capturingTouchForScrollView && this->scrollView)
				this->scrollView->takeTouch(touch, TouchPhase::Cancelled);

			if (!this->capturingTouchForScrollView && this->singleTouchDelegate)
			{
				this->singleTouchDelegate(this, touch, TouchPhase::Cancelled);
				this->onTouchEffect(TouchPhase::Cancelled);
			}

			this->capturingTouchForScrollView = false;
		};
	}

	void Button::initMultiTouchListener(void)
	{
		this->listenerMultiTouch = EventListenerTouchAllAtOnce::create();
		this->listenerMultiTouch->retain();

		this->listenerMultiTouch->onTouchesBegan = [this](const std::vector<Touch *> &touches, Event *event)
		{
			bool acceptingTouch = this->isAcceptingAllTouches();
			if (!acceptingTouch)
			{
				for (int i = 0, c = touches.size(); i < c; ++i)
				{
					auto touch = touches[i];
					if (this->isInsideBounds(touch))
					{
						this->multiTouchId = touch->getID();
						acceptingTouch = true;
						break;
					}
				}
			}

			if (!acceptingTouch)
				return;

			if (this->multiTouchDelegate)
				this->multiTouchDelegate(this, touches, TouchPhase::Began);

			this->onTouchEffect(TouchPhase::Began);
		};
		this->listenerMultiTouch->onTouchesMoved = [this](const std::vector<Touch *> &touches, Event *event)
		{
			if (this->multiTouchDelegate)
				this->multiTouchDelegate(this, touches, TouchPhase::Moved);

			this->onTouchEffect(TouchPhase::Moved);
		};
		this->listenerMultiTouch->onTouchesEnded = [this](const std::vector<Touch *> &touches, Event *event)
		{
			bool acceptingTouch = this->isAcceptingAllTouches();
			if (!acceptingTouch)
			{
				for (int i = 0, c = touches.size(); i < c; ++i)
				{
					auto touch = touches[i];
					if (touch->getID() == this->multiTouchId)
					{
						acceptingTouch = true;
						break;
					}
				}
			}

			if (!acceptingTouch)
				return;

			if (this->multiTouchDelegate)
				this->multiTouchDelegate(this, touches, TouchPhase::Ended);

			this->onTouchEffect(TouchPhase::Ended);
		};
		this->listenerMultiTouch->onTouchesCancelled = [this](const std::vector<Touch *> &touches, Event *event)
		{
			bool acceptingTouch = this->isAcceptingAllTouches();
			if (!acceptingTouch)
			{
				for (int i = 0, c = touches.size(); i < c; ++i)
				{
					auto touch = touches[i];
					if (touch->getID() == this->multiTouchId)
					{
						acceptingTouch = true;
						break;
					}
				}
			}

			if (!acceptingTouch)
				return;
			
			if (this->multiTouchDelegate)
				this->multiTouchDelegate(this, touches, TouchPhase::Cancelled);

			this->onTouchEffect(TouchPhase::Cancelled);
		};
	}

	void Button::setTouchEnabled(bool enabled)
	{
		if (this->touchEnabled == enabled && (this->listenerSingleTouch || this->listenerMultiTouch))
			return;

		this->touchEnabled = enabled;

		if (enabled)
		{
			if (this->multitouch)
				this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listenerMultiTouch, this);
			else
				this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->listenerSingleTouch, this);
		}
		else
		{
			if (this->multitouch)
				this->_eventDispatcher->removeEventListener(this->listenerMultiTouch);
			else
				this->_eventDispatcher->removeEventListener(this->listenerSingleTouch);
		}
	}

	void Button::replaceSpriteFrame(cocos2d::SpriteFrame *spriteFrame)
	{
		if (this->popoutSprite)
		{
			this->popoutSprite->removeFromParent();
			this->popoutSprite = nullptr;
		}

		if (spriteFrame)
		{
			this->setSpriteFrame(spriteFrame);
			this->setPopout(this->popout);
		}
	}

	void Button::setPopout(bool popout)
	{
		if (this->popout == popout && !(popout && !this->popoutSprite))
			return;

		this->popout = popout;

		if (popout)
		{
			auto spriteFrame = this->getSpriteFrame();
			if (spriteFrame)
			{
				auto sprite = Sprite::createWithSpriteFrame(spriteFrame);
				this->addChild(sprite, 0);
				sprite->setPosition(Point(this->_contentSize.width / 2, this->_contentSize.height / 2));
				this->popoutSprite = sprite;
				this->popoutSprite->setVisible(false);

				this->popoutScale = (sprite->getContentSize().width + 4) / sprite->getContentSize().width;
			}
		}
		else
		{
			if (this->popoutSprite)
			{
				this->popoutSprite->removeFromParent();
				this->popoutSprite = nullptr;
			}
		}
	}

	Label * Button::addLabelTTF(const std::string &labelString, const std::string &font, int fontSize)
	{
		auto label = Label::createWithTTF(labelString, font, fontSize);
		if (label)
		{
			label->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
			this->addChild(label);
		}
		return label;
	}

	Label * Button::addLabelBMP(const std::string &labelString, const std::string &font)
	{
		auto label = Label::createWithBMFont(font, labelString);
		if (label)
		{
			label->setPosition(this->getContentSize().width / 2, this->getContentSize().height / 2);
			this->addChild(label);
		}
		return label;
	}

	void Button::onTouchEffect(TouchPhase phase, bool autoEnd)
	{
		if (!this->popout || !this->popoutSprite)
			return;

		if (phase == TouchPhase::Began)
		{
			auto actBegan = this->popoutSprite->getActionByTag(kTagPopoutBegan);
			auto actEnded = this->popoutSprite->getActionByTag(kTagPopoutEnded);

			if (actBegan)
				return;

			if (actEnded)
				this->popoutSprite->stopAction(actEnded);

			if (autoEnd)
				actBegan = Sequence::create(
					Show::create(),
					ScaleTo::create(POPOUT_DURATION, this->popoutScale),
					ScaleTo::create(POPOUT_DURATION, 1),
					Hide::create(),
					nullptr);
			else
				actBegan = Sequence::create(
					Show::create(),
					ScaleTo::create(POPOUT_DURATION, this->popoutScale),
					nullptr);

			actBegan->setTag(autoEnd ? kTagPopoutEnded : kTagPopoutBegan);
			this->popoutSprite->runAction(actBegan);
		}
		else if (phase == TouchPhase::Ended || phase == TouchPhase::Cancelled)
		{
			auto actBegan = this->popoutSprite->getActionByTag(kTagPopoutBegan);
			auto actEnded = this->popoutSprite->getActionByTag(kTagPopoutEnded);

			if (actEnded)
				return;

			if (actBegan)
				this->popoutSprite->stopAction(actBegan);

			actEnded = Sequence::create(
				ScaleTo::create(POPOUT_DURATION, 1),
				Hide::create(),
				nullptr);

			actEnded->setTag(kTagPopoutEnded);
			this->popoutSprite->runAction(actEnded);
		}
	}

	Node * Button::InflaterDelegate::createFromData(UIInflater *inflater, const ValueMap &data)
	{
		ValueMap::const_iterator it;
		std::string fileName, frameName;
		bool popout = false, multitouch = false;

		if ((it = data.find("fileName")) != data.end())
			fileName = it->second.asString();
		else if ((it = data.find("frameName")) != data.end())
			frameName = it->second.asString();
		else
			return nullptr;

		popout = ValueMapUtils::tryGetBool("popout", data);
		multitouch = ValueMapUtils::tryGetBool("multitouch", data);

		Button *button = nullptr;
		if (fileName.length())
			button = multitouch ?
				Button::createWithFile(fileName.c_str(), static_cast<Button::MultiTouchDelegate>(nullptr), popout) :
				Button::createWithFile(fileName.c_str(), static_cast<Button::SingleTouchDelegate>(nullptr), popout);
		else if (frameName.length())
			button = multitouch ?
				Button::createWithSpriteFrameName(frameName.c_str(), static_cast<Button::MultiTouchDelegate>(nullptr), popout) :
				Button::createWithSpriteFrameName(frameName.c_str(), static_cast<Button::SingleTouchDelegate>(nullptr), popout);
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
