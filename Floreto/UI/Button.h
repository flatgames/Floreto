#ifndef __FLORETO_BUTTON_H__
#define __FLORETO_BUTTON_H__

#include "../Common.h"

#include "TouchPhase.h"
#include "TouchProtocol.h"
#include "ScrollViewProtocol.h"
#include "UIElementProtocol.h"
#include "UIInflater.h"

#include <functional>

namespace Floreto
{
	class Button :
		public cocos2d::Sprite,
		public TouchProtocol,
		public UIElementProtocol,
		public ScrollViewAwareProtocol
	{
		DeclareInflaterDelegate(Button, InflaterDelegate);

	public:
		typedef std::function<bool(Button *button, cocos2d::Touch *touch, TouchPhase phase)> SingleTouchDelegate;
		typedef std::function<void(Button *button, const std::vector<cocos2d::Touch*> &touches, TouchPhase phase)> MultiTouchDelegate;
		typedef std::function<void(void)> SimpleDelegate;

	public:
		static Button * create(const SingleTouchDelegate &delegate, bool popout = true);
		static Button * createWithFile(const char *filename, const SingleTouchDelegate &delegate, bool popout = true);
		static Button * createWithSpriteFrameName(const char *spriteFrameName, const SingleTouchDelegate &delegate, bool popout = true);
		static Button * createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, const SingleTouchDelegate &delegate, bool popout = true);

		static Button * create(const MultiTouchDelegate &delegate, bool popout = true);
		static Button * createWithFile(const char *filename, const MultiTouchDelegate &delegate, bool popout = true);
		static Button * createWithSpriteFrameName(const char *spriteFrameName, const MultiTouchDelegate &delegate, bool popout = true);
		static Button * createWithSpriteFrame(cocos2d::SpriteFrame *spriteFrame, const MultiTouchDelegate &delegate, bool popout = true);

		static SingleTouchDelegate createSimpleDelegate(const SimpleDelegate &delegate);

		static bool isInsideBounds(cocos2d::Node *node, float x, float y);
		static bool isInsideBounds(cocos2d::Node *node, cocos2d::Touch *touch);
		bool isInsideBounds(float x, float y);
		bool isInsideBounds(cocos2d::Touch *touch);

	public:
		Button(void);
		virtual ~Button(void);

		virtual void onEnter(void);
		virtual void onExit(void);
		virtual void cleanup(void);

	protected:
		virtual bool init(cocos2d::SpriteFrame *spriteFrame, const SingleTouchDelegate &singleTouchDelegate, const MultiTouchDelegate &multiTouchDelegate, bool multitouch, bool popout);

	public:
		inline virtual const SingleTouchDelegate & getSingleTouchDelegate(void) const
		{
			return this->singleTouchDelegate;
		}

		virtual void setSingleTouchDelegate(const SingleTouchDelegate &delegate, bool forceful = false);

		inline virtual const  MultiTouchDelegate & getMultiTouchDelegate(void) const
		{
			return this->multiTouchDelegate;
		}

		virtual void setMultiTouchDelegate(const MultiTouchDelegate &delegate, bool forceful = false);

		inline virtual bool isMultitouch(void) const
		{
			return this->multitouch;
		}

		// Returns true if the button accepts all touches at the same time.
		// Only applicable in multi-touch mode.
		inline virtual bool isAcceptingAllTouches(void)
		{
			return this->acceptingAllTouches;
		}

		// In multi-touch mode, setting this value to true makes the button
		// accept all touches at the same time, even if the touch positions
		// are outside the bounding box of the button.
		inline virtual void setAcceptingAllTouches(bool value)
		{
			this->acceptingAllTouches = value;
		}

		inline virtual bool isTouchEnabled(void) const override
		{
			return this->touchEnabled;
		}

		virtual void setTouchEnabled(bool enabled) override;

		virtual void replaceSpriteFrame(cocos2d::SpriteFrame *spriteFrame);

		inline virtual bool isPopout(void) const
		{
			return this->popout;
		}

		virtual void setPopout(bool popout);

		virtual cocos2d::Label * addLabelTTF(const std::string &labelString, const std::string &font, int fontSize);
		virtual cocos2d::Label * addLabelBMP(const std::string &labelString, const std::string &font);

	protected:
		virtual void setMultitouch(bool multitouch);
		virtual void initSingleTouchListener(void);
		virtual void initMultiTouchListener(void);
		
	protected:
		virtual void onTouchEffect(TouchPhase phase, bool autoEnd = false);

	protected:
		bool touchEnabled;
		bool multitouch;
		bool acceptingAllTouches;
		int multiTouchId;

		SingleTouchDelegate singleTouchDelegate;
		MultiTouchDelegate multiTouchDelegate;
		
		union
		{
			cocos2d::EventListenerTouchOneByOne *listenerSingleTouch;
			cocos2d::EventListenerTouchAllAtOnce *listenerMultiTouch;
		};

		bool popout;
		cocos2d::Sprite *popoutSprite;
		float popoutScale;

		// The x- or y-coordinate of the touch point when Began event is received.
		// Whether it is x or y depends on the orientation of the scrollview.
		float orgTouchValue;

		// Touch focus is captured just to control the scrollview.
		// Other button delegates will not be invoked.
		bool capturingTouchForScrollView;

		// Touch moved events are captured to control the scrollview.
		bool capturingMovesForScrollView;
	};

// Short-hand version for declaration of a lambda as Single Touch Delegate
#define MakeSingleTouchDelegate(__closure__, __button__, __touch__, __phase__) \
	__closure__(Floreto::Button *__button__, cocos2d::Touch *__touch__, Floreto::TouchPhase __phase__) -> bool

// Short-hand version for declaration of a lambda as Multi Touch Delegate
#define MakeMultiTouchDelegate(__closure__, __button__, __touches__, __phase__) \
	__closure__(Floreto::Button *__button__, const std::vector<cocos2d::Touch*> &__touches__, Floreto::TouchPhase __phase__) -> void

}

#endif//__FLORETO_BUTTON_H__
