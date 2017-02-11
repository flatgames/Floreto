#ifndef __FLORETO_TOGGLEBUTTON_H__
#define __FLORETO_TOGGLEBUTTON_H__

#include "Button.h"

namespace Floreto
{

	class ToggleButton : public Button
	{
		DeclareInflaterDelegate(ToggleButton, InflaterDelegate);

	public:
		typedef std::function<void(ToggleButton *button, bool state)> ToggleDelegate;

	public:
		static ToggleButton * createWithFile(const char *onFile, const char *offFile, const ToggleDelegate &delegate, bool multitouch = false);
		static ToggleButton * createWithSpriteFrameName(const char *onSpriteFrameName, const char *offSpriteFrameName, const ToggleDelegate &delegate, bool multitouch = false);
		static ToggleButton * createWithSpriteFrame(cocos2d::SpriteFrame *onSpriteFrame, cocos2d::SpriteFrame *offSpriteFrame, const ToggleDelegate &delegate, bool multitouch = false);

	public:
		ToggleButton(void);
		virtual ~ToggleButton(void);

	protected:
		virtual bool init(cocos2d::SpriteFrame *onFrame, cocos2d::SpriteFrame *offFrame, const ToggleDelegate &delegate, bool multitouch);

	public:
		inline virtual bool isOn(void) const
		{
			return this->toggleState;
		}
		
		virtual void setOn(bool value);

		virtual void toggle(void);

		inline const ToggleDelegate & getToggleDelegate(void) const
		{
			return this->toggleDelegate;
		}

		inline void setToggleDelegate(const ToggleDelegate &delegate)
		{
			this->toggleDelegate = delegate;
		}

	public:
		virtual void setSingleTouchDelegate(const SingleTouchDelegate &delegate, bool forceful = false) override
		{
		}

		virtual void setMultiTouchDelegate(const MultiTouchDelegate &delegate, bool forceful = false) override
		{
		}

		virtual void replaceSpriteFrame(cocos2d::SpriteFrame *spriteFrame) override
		{
		}

	protected:
		virtual void onTouchEffect(TouchPhase phase, bool autoEnd = false) override
		{
		}

	protected:
		ToggleDelegate toggleDelegate;
		bool toggleState;

		cocos2d::SpriteFrame *onFrame;
		cocos2d::SpriteFrame *offFrame;
	};

}

#endif//__FLORETO_TOGGLEBUTTON_H__
