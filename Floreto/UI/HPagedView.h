
#ifndef __FLORETO_HPAGEVIEW_H__
#define __FLORETO_HPAGEVIEW_H__

#include "../Common.h"
#include "ScrollViewProtocol.h"
#include "TouchProtocol.h"
#include "UIElementProtocol.h"
#include "UIInflater.h"

namespace Floreto
{

	class HPagedView :
		public cocos2d::Node,
		public TouchProtocol,
		public ScrollViewProtocol,
		public UIElementProtocol
	{
	public:
		typedef std::function<void(HPagedView *scrollView, int oldIndex, int newIndex)> PageChangedDelegate;

	protected:
		enum class MotionDirection
		{
			None,
			Left,
			Right
		};

	public:
		static HPagedView * create(const cocos2d::Size &size, const PageChangedDelegate &pageChangedDelegate);

		HPagedView(void);
		virtual ~HPagedView(void);

		virtual void onEnter(void) override;
		virtual void onExit(void) override;
		virtual void cleanup(void) override;

		virtual void takeTouch(cocos2d::Touch *touch, TouchPhase phase) override;
		virtual bool isInsideView(cocos2d::Touch *touch) override;

		ScrollViewProtocol::Orientation getOrientation(void)
		{
			return ScrollViewProtocol::Orientation::Horizontal;
		}

		float getScrollThreshold(void) override
		{
			return 5;
		}

		virtual void quickSet(void);
		virtual int getCurrentPage(void) const { return currentIndex; }
		virtual void setCurrentPage(int index);
		virtual void setCurrentPageByScrolling(int index);

		virtual void addContentPage(cocos2d::Node *page);
		virtual int getPageCount(void) const { return contentPages.size(); }

		virtual void update(float dt);
		virtual void visit(cocos2d::Renderer *renderer, const cocos2d::Mat4 &parentTransform, uint32_t parentFlags) override;

		virtual bool isTouchEnabled(void) const override;
		virtual void setTouchEnabled(bool enabled) override;

		virtual bool isUsingStencil(void) const final;
		virtual void setUsingStencil(cocos2d::Node *stencil) final;

		virtual cocos2d::ClippingNode * getClippingNode(void) final;

	protected:
		bool init(const cocos2d::Size &size, const PageChangedDelegate &pageChangedDelegate);

		virtual void startMotion(float t, float span, float xSrc, float xDest);
		virtual void resumeMotion(void);
		virtual void stopMotion(void);
		virtual void haltMotion(void);

		bool onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event);
		void onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event);
		void onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event);

	protected:
		cocos2d::Node *content;
		std::vector<cocos2d::Node *> contentPages;

		cocos2d::EventListenerTouchOneByOne *touchListener;
		bool touchEnabled;

		float xSrc;
		float xDest;
		float xPivot;
		float xTouch0;
		float yTouch0;
		float xLast;
		float xTouchBegan;
		float span;
		float tMove;
		float acctMove;

		MotionDirection direction;
		bool inMotion;
		bool suspended;

		int currentIndex;
		int newIndex;

		cocos2d::CustomCommand _beforeVisitCmdScissor;
		cocos2d::CustomCommand _afterVisitCmdScissor;
		float _viewScaleX;
		float _viewScaleY;

		cocos2d::ClippingNode *clippingNode;
		bool usingStencil;

		PageChangedDelegate pageChangedDelegate;
	};

}

#endif//__FLORETO_HPAGEVIEW_H__
