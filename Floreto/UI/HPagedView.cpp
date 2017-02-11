#include "HPagedView.h"
#include <limits>

using namespace cocos2d;

namespace Floreto
{
	HPagedView * HPagedView::create(const cocos2d::Size &size, const PageChangedDelegate &pageChangedDelegate)
	{
		HPagedView *view = new HPagedView();
		if (view && view->init(size, pageChangedDelegate))
		{
			view->autorelease();
			return view;
		}

		CC_SAFE_DELETE(view);
		return nullptr;
	}

	bool HPagedView::init(const cocos2d::Size &size, const PageChangedDelegate &pageChangedDelegate)
	{
		if (!Node::init())
			return false;

		this->setContentSize(size);
		this->setCascadeOpacityEnabled(true);

		this->pageChangedDelegate = pageChangedDelegate;

		auto content = Node::create();
		content->setCascadeOpacityEnabled(true);

		this->addChild(content);
		this->content = content;
		content->retain();

		content->setAnchorPoint(Vec2::ZERO);
		content->setPosition(Vec2::ZERO);

		this->touchListener = EventListenerTouchOneByOne::create();
		this->touchListener->retain();

		this->touchListener->setSwallowTouches(true);
		this->touchListener->onTouchBegan = CC_CALLBACK_2(HPagedView::onTouchBegan, this);
		this->touchListener->onTouchMoved = CC_CALLBACK_2(HPagedView::onTouchMoved, this);
		this->touchListener->onTouchEnded = CC_CALLBACK_2(HPagedView::onTouchEnded, this);
		this->touchListener->onTouchCancelled = CC_CALLBACK_2(HPagedView::onTouchEnded, this);

		return true;
	}

	HPagedView::HPagedView()
		: content(nullptr), touchListener(nullptr), touchEnabled(false),
		xSrc(0), xDest(0), xPivot(0), xTouch0(0), yTouch0(0), xLast(0), xTouchBegan(0),
		span(0), tMove(0), acctMove(0),
		direction(MotionDirection::None), inMotion(false), suspended(false),
		currentIndex(-1), newIndex(0),
		clippingNode(nullptr), usingStencil(false)
	{
	}

	HPagedView::~HPagedView(void)
	{
		CC_SAFE_RELEASE_NULL(content);
		CC_SAFE_RELEASE_NULL(touchListener);
	}

	void HPagedView::startMotion(float t, float span, float xSrc, float xDest)
	{
		if (!this->inMotion)
		{
			this->tMove = t;
			this->span = span;
			this->xSrc = xSrc;
			this->xDest = xDest;

			this->acctMove = 0;
			this->scheduleUpdate();

			this->inMotion = true;
			this->suspended = false;
		}
	}

	void HPagedView::resumeMotion(void)
	{
		if (!this->inMotion)
		{
			this->scheduleUpdate();
			this->inMotion = true;
			this->suspended = false;
		}
	}

	void HPagedView::stopMotion(void)
	{
		if (this->inMotion)
		{
			this->unscheduleUpdate();
			this->acctMove = 0;

			this->inMotion = false;
			this->suspended = false;
			this->direction = MotionDirection::None;
			this->xPivot = this->xDest;

			int oldIndex = this->currentIndex;
			this->currentIndex = this->newIndex;

			if (this->pageChangedDelegate)
				this->pageChangedDelegate(this, oldIndex, this->newIndex);
		}
	}

	void HPagedView::haltMotion(void)
	{
		if (this->inMotion)
		{
			this->unscheduleUpdate();
			this->inMotion = false;
			this->suspended = true;
		}
	}

	void HPagedView::takeTouch(cocos2d::Touch *touch, TouchPhase phase)
	{
		switch (phase)
		{
		case Floreto::TouchPhase::Began:
			this->onTouchBegan(touch, nullptr);
			break;
		case Floreto::TouchPhase::Moved:
			this->onTouchMoved(touch, nullptr);
			break;
		case Floreto::TouchPhase::Ended:
		case Floreto::TouchPhase::Cancelled:
			this->onTouchEnded(touch, nullptr);
			break;
		}
	}

	bool HPagedView::isInsideView(cocos2d::Touch *touch)
	{
		Vec2 p = touch->getLocation();
		p = this->getParent()->convertToNodeSpace(p);
		return this->getBoundingBox().containsPoint(p);
	}

	void HPagedView::quickSet(void)
	{
		this->stopMotion();
		this->content->setPositionX(this->xDest);
	}

	void HPagedView::setCurrentPage(int index)
	{
		float x = -this->getContentSize().width * index;
		this->content->setPositionX(x);

		this->xSrc = x;
		this->xDest = x;

		int oldIndex = this->currentIndex;
		this->currentIndex = index;

		if (this->pageChangedDelegate)
			this->pageChangedDelegate(this, oldIndex, index);
	}

	void HPagedView::setCurrentPageByScrolling(int index)
	{
		float x = -this->getContentSize().width * index;
		
		this->xSrc = this->content->getPositionX();
		this->xDest = x;
		float distance = this->xDest - this->xSrc;

		if (distance < 0)
		{
			// to the left (forward)
			if (this->direction == MotionDirection::Left)
			{
				this->xPivot = this->xDest;
				this->currentIndex = this->newIndex;
			}

			if (this->currentIndex == this->contentPages.size() - 1 || (distance > -80 && this->direction != MotionDirection::Right))
			{
				// under limit or current is last
				xDest = this->xPivot;
				this->newIndex = this->currentIndex;
				this->direction = MotionDirection::Right;
			}
			else
			{
				xDest = -this->getContentSize().width * (this->currentIndex + 1);
				this->newIndex = this->currentIndex + 1;
				this->direction = MotionDirection::Left;
			}
		}
		else
		{
			// to the right (backward)
			if (this->direction == MotionDirection::Right)
			{
				this->xPivot = this->xDest;
				this->currentIndex = this->newIndex;
			}

			if (this->currentIndex == 0 || (distance < 80 && this->direction == MotionDirection::Right))
			{
				// under limit or current is first
				xDest = this->xPivot;
				this->newIndex = this->currentIndex;
				this->direction = MotionDirection::Left;
			}
			else
			{
				xDest = -this->getContentSize().width * (this->currentIndex - 1);
				this->newIndex = this->currentIndex - 1;
				this->direction = MotionDirection::Right;
			}
		}

		float span = xDest - xSrc;
		float t = fabsf(span / this->getContentSize().width) * 0.5f;
		this->startMotion(t, span, xSrc, xDest);
	}

	void HPagedView::addContentPage(Node *page)
	{
		this->content->addChild(page);
		page->setAnchorPoint(Vec2::ZERO);
		page->setPosition(this->content->getContentSize().width, 0);

		this->content->setContentSize(Size(
			this->content->getContentSize().width + this->getContentSize().width,
			this->getContentSize().height));

		this->contentPages.push_back(page);

		if (this->currentIndex < 0)
			this->currentIndex = 0;
	}

	void HPagedView::update(float dt)
	{
		this->acctMove += dt;
		if (this->acctMove >= this->tMove)
		{
			this->content->setPositionX(this->xDest);
			this->stopMotion();
		}
		else
		{
			float p = this->acctMove / this->tMove;
			p = -p * (p - 2);
			this->content->setPositionX(this->xSrc + this->span * p);
		}
	}

	void HPagedView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
	{
		if (this->usingStencil)
		{
			Node::visit(renderer, parentTransform, parentFlags);
			return;
		}

		if (this->_transformDirty || (parentFlags & FLAGS_DIRTY_MASK))
		{
			_viewScaleX = 1;
			_viewScaleY = 1;
			for (Node *node = this; node; node = node->getParent())
			{
				_viewScaleX *= node->getScaleX();
				_viewScaleY *= node->getScaleY();
			}
		}

		_beforeVisitCmdScissor.init(_globalZOrder);
		_beforeVisitCmdScissor.func = [this]()
		{
			Point org = this->convertToWorldSpace(Point::ZERO);
			Size siz = this->getContentSize();

			glEnable(GL_SCISSOR_TEST);
			auto glview = Director::getInstance()->getOpenGLView();
			glview->setScissorInPoints(org.x, org.y, siz.width * this->_viewScaleX, siz.height * this->_viewScaleY);
		};
		renderer->addCommand(&_beforeVisitCmdScissor);

		Node::visit(renderer, parentTransform, parentFlags);

		_afterVisitCmdScissor.init(_globalZOrder);
		_afterVisitCmdScissor.func = []()
		{
			glDisable(GL_SCISSOR_TEST);
		};
		renderer->addCommand(&_afterVisitCmdScissor);
	}

	bool HPagedView::isUsingStencil(void) const
	{
		return this->usingStencil;
	}

	void HPagedView::setUsingStencil(Node *stencil)
	{
		if (stencil)
		{
			this->usingStencil = true;
			if (!this->clippingNode)
			{
				auto clippingNode = ClippingNode::create(stencil);
				clippingNode->setAlphaThreshold(0.9f);
				clippingNode->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
				clippingNode->setPosition(Vec2::ZERO);
				this->addChild(clippingNode);
				this->clippingNode = clippingNode;

				if (this->content)
				{
					this->content->retain();
					this->content->getParent()->removeChild(this->content, false);
					this->clippingNode->addChild(this->content);
					this->content->release();
				}
			}
			else
			{
				this->clippingNode->setStencil(stencil);
			}

			stencil->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
			stencil->setPosition(Vec2::ZERO);
		}
		else
		{
			this->usingStencil = false;
			if (this->clippingNode)
			{
				if (this->content)
				{
					this->content->retain();
					this->content->getParent()->removeChild(this->content, false);
					this->addChild(this->content);
					this->content->release();
				}

				this->clippingNode->removeFromParent();
				this->clippingNode = nullptr;
			}
		}
	}

	ClippingNode * HPagedView::getClippingNode(void)
	{
		return this->clippingNode;
	}

	bool HPagedView::isTouchEnabled(void) const
	{
		return this->touchEnabled;
	}

	void HPagedView::setTouchEnabled(bool value)
	{
		if (this->touchEnabled == value)
			return;

		this->touchEnabled = value;

		if (value)
		{
			this->_eventDispatcher->addEventListenerWithSceneGraphPriority(this->touchListener, this);
		}
		else
		{
			this->_eventDispatcher->removeEventListener(this->touchListener);
		}
	}

	void HPagedView::onEnter(void)
	{
		Node::onEnter();
	}

	void HPagedView::onExit(void)
	{
		this->setTouchEnabled(false);
		Node::onExit();
	}

	void HPagedView::cleanup(void)
	{
		this->setTouchEnabled(false);
		Node::cleanup();
	}

	bool HPagedView::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
	{
		if (this->currentIndex < 0 || this->isInsideView(touch) == false)
			return false;

		if (this->inMotion)
			this->haltMotion();

		float x = touch->getLocation().x, y = touch->getLocation().y;
		this->xTouch0 = x;
		this->yTouch0 = y;
		this->xLast = x;
		this->xTouchBegan = this->content->getPositionX();

		return true;
	}

	void HPagedView::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
	{
		float x = touch->getLocation().x;
		float deltaX = x - this->xLast;
		this->content->setPositionX(this->content->getPositionX() + deltaX);
		this->xLast = x;
	}

	void HPagedView::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
	{
		float x = touch->getLocation().x, y = touch->getLocation().y;
		float deltaX = x - this->xTouch0;
		float deltaY = y - this->yTouch0;

		if (fabsf(deltaX) < 10)
		{
			if (fabsf(deltaY) < 10)
			{
				this->haltMotion();
				if (this->suspended)
					this->resumeMotion();
				this->suspended = false;
			}
		}
		else
		{
			if (std::numeric_limits<float>::infinity() == this->xTouchBegan)
				this->xTouchBegan = this->xPivot;

			float xSrc = this->content->getPositionX();
			float xDest;
			float distance = xSrc - this->xTouchBegan;

			if (distance < 0)
			{
				// to the left (forward)
				if (this->direction == MotionDirection::Left)
				{
					this->xPivot = this->xDest;
					this->currentIndex = this->newIndex;
				}

				if (this->currentIndex == this->contentPages.size() - 1 || (distance > -80 && this->direction != MotionDirection::Right))
				{
					// under limit or current is last
					xDest = this->xPivot;
					this->newIndex = this->currentIndex;
					this->direction = MotionDirection::Right;
				}
				else
				{
					xDest = -this->getContentSize().width * (this->currentIndex + 1);
					this->newIndex = this->currentIndex + 1;
					this->direction = MotionDirection::Left;
				}
			}
			else
			{
				// to the right (backward)
				if (this->direction == MotionDirection::Right)
				{
					this->xPivot = this->xDest;
					this->currentIndex = this->newIndex;
				}

				if (this->currentIndex == 0 || (distance < 80 && this->direction == MotionDirection::Right))
				{
					// under limit or current is first
					xDest = this->xPivot;
					this->newIndex = this->currentIndex;
					this->direction = MotionDirection::Left;
				}
				else
				{
					xDest = -this->getContentSize().width * (this->currentIndex - 1);
					this->newIndex = this->currentIndex - 1;
					this->direction = MotionDirection::Right;
				}
			}

			float span = xDest - xSrc;
			float t = fabsf(span / this->getContentSize().width) * 0.5f;
			this->startMotion(t, span, xSrc, xDest);
		}

		this->xTouchBegan = std::numeric_limits<float>::infinity();
	}

}
