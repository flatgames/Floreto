#ifndef __FLORETO_VSCROLLVIEW_H__
#define __FLORETO_VSCROLLVIEW_H__

#include "../Common.h"

#include "ScrollViewProtocol.h"
#include "TouchProtocol.h"
#include "UIElementProtocol.h"
#include "UIInflater.h"

namespace Floreto
{

	class VScrollView :
		public cocos2d::Node,
		public TouchProtocol,
		public ScrollViewProtocol,
		public UIElementProtocol
	{
		DeclareInflaterDelegate(VScrollView, InflaterDelegate);

	public:
		static VScrollView * create(const cocos2d::Size &size, cocos2d::Node *content);

		VScrollView(void);
		virtual ~VScrollView(void);

		virtual void refresh(void);
		virtual void updateBounds(void);
		virtual void takeTouch(cocos2d::Touch *touch, TouchPhase phase) override;
		virtual bool isInsideView(cocos2d::Touch *touch) override;

		inline virtual cocos2d::Node * getContentNode(void)
		{
			return this->content;
		}

		virtual void setContentNode(cocos2d::Node *node);

		virtual float getScrollOffset(void) const;
		virtual void setScrollOffset(float offset);

		ScrollViewProtocol::Orientation getOrientation(void) override
		{
			return ScrollViewProtocol::Orientation::Vertical;
		}

		float getScrollThreshold(void) override
		{
			return scrollThreshold;
		}

		void setScrollThreshold(float value)
		{
			scrollThreshold = value;
		}

		virtual void update(float dt);
		virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;
		virtual void setContentSize(const cocos2d::Size &size) override;

		virtual void setTouchEnabled(bool value) override;
		virtual bool isTouchEnabled(void) const override { return this->touchEnabled; }

		virtual void onEnter(void) override;
		virtual void onExit(void) override;
		virtual void cleanup(void) override;

		// Sets a scrollpad for the view.
		// padding: how far is it from the border.
		// otherSide: put it on the left.
		// consistent: leave it visible all the time.
		void setScrollPad(cocos2d::Node *scrollPad, float padding, bool otherSide, bool consistent);

	protected:
		bool init(const cocos2d::Size &size, cocos2d::Node *content);

		virtual void startMotion(float velocity);
		virtual void stopMotion(void);

		void showScrollPad(void);
		void hideScrollPad(void);
		void updateScrollPad(float y);

		bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
		void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
		void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	protected:
		cocos2d::Node *content;
		cocos2d::EventListenerTouchOneByOne *touchListener;
		bool touchEnabled;

		float lowerPadding;
		float upperPadding;
		float lowerBound;
		float upperBound;
		float lastDeltaScroll;
		float xLast;
		float yLast;
		float xTouch0;
		float yTouch0;
		float tMotion0;
		float velocity;
		bool inMotion;
		bool suspended;

		clock_t tMove;
		clock_t tTouch0;

		float scrollThreshold;

		cocos2d::Node *scrollPad;
		bool scrollPadOtherSide;
		float scrollPadPadding;
		bool scrollPadConsistent;

		cocos2d::CustomCommand _beforeVisitCmdScissor;
		cocos2d::CustomCommand _afterVisitCmdScissor;
		float _viewScaleX;
		float _viewScaleY;
	};
}

#endif//__FLORETO_VSCROLLVIEW_H__
