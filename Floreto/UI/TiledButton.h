#ifndef __FLORETO_TILEDBUTTON_H__
#define __FLORETO_TILEDBUTTON_H__

#include "Button.h"

namespace Floreto
{
	class TiledButton : public Button
	{
		DeclareInflaterDelegate(TiledButton, InflaterDelegate);

	public:
		static TiledButton * createWithFile(const char *normalFile, const char *pressedFile, const SingleTouchDelegate &delegate);
		static TiledButton * createWithSpriteFrameName(const char *normalSpriteFrameName, const char *pressedSpriteFrameName, const SingleTouchDelegate &delegate);
		static TiledButton * createWithSpriteFrame(cocos2d::SpriteFrame *normalSpriteFrame, cocos2d::SpriteFrame *pressedSpriteFrame, const SingleTouchDelegate &delegate);

		static TiledButton * createWithFile(const char *normalFile, const char *pressedFile, const MultiTouchDelegate &delegate);
		static TiledButton * createWithSpriteFrameName(const char *normalSpriteFrameName, const char *pressedSpriteFrameName, const MultiTouchDelegate &delegate);
		static TiledButton * createWithSpriteFrame(cocos2d::SpriteFrame *normalSpriteFrame, cocos2d::SpriteFrame *pressedSpriteFrame, const MultiTouchDelegate &delegate);

	public:
		TiledButton(void);
		virtual ~TiledButton(void);

	protected:
		virtual bool init(cocos2d::SpriteFrame *normalSpriteFrame, cocos2d::SpriteFrame *pressedSpriteFrame, const SingleTouchDelegate &singleTouchDelegate, const MultiTouchDelegate &multiTouchDelegate, bool multitouch);

	public:
		virtual void replaceSpriteFrame(cocos2d::SpriteFrame *spriteFrame) override;

	protected:
		virtual void onTouchEffect(TouchPhase phase, bool autoEnd = false) override;

	protected:
		cocos2d::SpriteFrame *normalFrame;
		cocos2d::SpriteFrame *pressedFrame;
	};
}

#endif//__FLORETO_TILEDBUTTON_H__
