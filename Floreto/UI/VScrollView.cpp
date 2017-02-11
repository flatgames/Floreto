#include "VScrollView.h"
#include "../Utils/ClassModel.h"
#include "../Utils/ValueMapUtils.h"

using namespace cocos2d;

namespace Floreto
{

	VScrollView * VScrollView::create(const cocos2d::Size &size, cocos2d::Node *content)
	{
		VScrollView *view = new VScrollView();
		if (view && view->init(size, content))
		{
			view->autorelease();
			return view;
		}

		CC_SAFE_DELETE(view);
		return nullptr;
	}

	bool VScrollView::init(const cocos2d::Size &size, cocos2d::Node *content)
	{
		if (!Node::init())
			return false;

		this->setContentSize(size);

		if (content)
		{
			this->addChild(content);
			this->content = content;
			content->retain();

			content->setAnchorPoint(Point::ZERO);
			content->setPosition(Point::ZERO);
		}

		this->touchListener = EventListenerTouchOneByOne::create();
		this->touchListener->retain();

		this->touchListener->setSwallowTouches(true);
		this->touchListener->onTouchBegan = CC_CALLBACK_2(VScrollView::onTouchBegan, this);
		this->touchListener->onTouchMoved = CC_CALLBACK_2(VScrollView::onTouchMoved, this);
		this->touchListener->onTouchEnded = CC_CALLBACK_2(VScrollView::onTouchEnded, this);
		this->touchListener->onTouchCancelled = CC_CALLBACK_2(VScrollView::onTouchEnded, this);

		return true;
	}

	VScrollView::VScrollView()
		: content(nullptr), touchEnabled(false), touchListener(nullptr),
		lowerPadding(0), upperPadding(0), lowerBound(0), upperBound(0),
		lastDeltaScroll(0), xLast(0), yLast(0), xTouch0(0), yTouch0(0), tMotion0(0), velocity(0),
		inMotion(false), suspended(false), tMove(0), tTouch0(0), scrollThreshold(5),
		scrollPad(nullptr), scrollPadConsistent(false)
	{
		this->touchEnabled = false;
		this->_viewScaleX = 1;
		this->_viewScaleY = 1;
	}

	VScrollView::~VScrollView(void)
	{
		CC_SAFE_RELEASE_NULL(content);
		CC_SAFE_RELEASE_NULL(touchListener);
		CC_SAFE_RELEASE_NULL(scrollPad);
	}

	void VScrollView::setScrollPad(cocos2d::Node *scrollPad, float padding, bool otherSide, bool consistent)
	{
		if (this->scrollPad)
		{
			this->scrollPad->removeFromParent();
			CC_SAFE_RELEASE_NULL(this->scrollPad);
		}

		this->addChild(scrollPad, 1);
		this->scrollPad = scrollPad;
		this->scrollPad->retain();

		scrollPad->setAnchorPoint(Point::ZERO);
		scrollPad->setPosition(
			otherSide ? padding : this->getContentSize().width - padding - scrollPad->getContentSize().width,
			this->getContentSize().height - scrollPad->getContentSize().height);

		this->scrollPadConsistent = consistent;
		this->scrollPadOtherSide = otherSide;
		this->scrollPadPadding = padding;

		if (!consistent)
			scrollPad->setOpacity(0);
	}

	void VScrollView::startMotion(float velocity)
	{
		if (!this->inMotion)
		{
			this->tMotion0 = 0;
			this->velocity = velocity;
			this->scheduleUpdate();
			this->inMotion = true;
			this->showScrollPad();
		}
	}

	void VScrollView::stopMotion(void)
	{
		if (this->inMotion)
		{
			this->unscheduleUpdate();
			this->tMotion0 = 0;
			this->velocity = 0;
			this->inMotion = false;
		}
		this->hideScrollPad();
	}

	void VScrollView::refresh(void)
	{
		this->updateBounds();

		if (content)
			this->content->setPositionY(this->lowerBound);
	}

	void VScrollView::updateBounds(void)
	{
		CCASSERT(content != nullptr, "NullReferenceException: content node of scrollview is null.");

		float contentHeight = this->content->getContentSize().height;
		float frameHeight = this->getContentSize().height;

		// how far to the bottom
		if (contentHeight < frameHeight)
		{
			this->lowerBound = frameHeight - contentHeight - this->upperPadding;
			this->upperBound = this->lowerBound;
		}
		else
		{
			this->lowerBound = frameHeight - contentHeight - this->upperPadding;
			this->upperBound = this->lowerPadding;
		}
	}

	void VScrollView::takeTouch(Touch *touch, TouchPhase phase)
	{
		switch (phase)
		{
			case Floreto::TouchPhase::Began:
			{
				this->onTouchBegan(touch, nullptr);
				break;
			}
			case Floreto::TouchPhase::Moved:
			{
				this->onTouchMoved(touch, nullptr);
				break;
			}
			case Floreto::TouchPhase::Ended:
			case Floreto::TouchPhase::Cancelled:
			{
				this->onTouchEnded(touch, nullptr);
				break;
			}
			default:
			{
				break;
			}
		}
	}

	bool VScrollView::isInsideView(Touch *touch)
	{
		Point p = this->getParent()->convertToNodeSpace(touch->getLocation());
		return this->getBoundingBox().containsPoint(p);
	}

	void VScrollView::setContentNode(cocos2d::Node *node)
	{
		if (this->content)
		{
			this->content->removeFromParent();
			this->content->release();
			this->content = nullptr;
		}

		this->content = node;
		this->content->retain();
		this->addChild(this->content);

		this->content->setAnchorPoint(Point::ZERO);
		this->content->setPosition(Point::ZERO);
	}

	float VScrollView::getScrollOffset(void) const
	{
		return this->content->getPositionY();
	}

	void VScrollView::setScrollOffset(float offset)
	{
		if (offset < this->lowerBound)
			offset = this->lowerBound;
		else if (offset > this->upperBound)
			offset = this->upperBound;

		this->content->setPositionY(offset);
	}

	void VScrollView::update(float dt)
	{
		CCASSERT(content != nullptr, "NullReferenceException: content node of scrollview is null.");

		float velocity = this->velocity;
		float y = this->content->getPositionY();

		// friction
		velocity *= 0.95f;

		// if too small, let velocity be 0
		if (fabsf(velocity) < 0.8f)
			velocity = 0;

		// update position or stop motion
		if (velocity)
			y += velocity * dt;
		else
			this->stopMotion();

		// stop at boundaries
		if (y < this->lowerBound)
		{
			y = this->lowerBound;
			this->stopMotion();
		}
		else if (y > this->upperBound)
		{
			y = this->upperBound;
			this->stopMotion();
		}

		// update properties
		this->velocity = velocity;
		this->content->setPositionY(y);

		this->updateScrollPad(y);
	}

	void VScrollView::visit(Renderer *renderer, const Mat4 &parentTransform, uint32_t parentFlags)
	{
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

	void VScrollView::setContentSize(const Size &size)
	{
		Node::setContentSize(size);

		if (scrollPad)
		{
			scrollPad->setAnchorPoint(Point::ZERO);
			scrollPad->setPosition(
				scrollPadOtherSide ? scrollPadPadding : this->getContentSize().width - scrollPadPadding - scrollPad->getContentSize().width,
				this->getContentSize().height - scrollPad->getContentSize().height);
		}
	}

	void VScrollView::setTouchEnabled(bool value)
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

	void VScrollView::onEnter(void)
	{
		Node::onEnter();
	}

	void VScrollView::onExit(void)
	{
		this->setTouchEnabled(false);
		Node::onExit();
	}

	void VScrollView::cleanup(void)
	{
		this->setTouchEnabled(false);
		Node::cleanup();
	}

	bool VScrollView::onTouchBegan(Touch *touch, Event *event)
	{
		if (!this->isInsideView(touch))
			return false;

		if (this->inMotion)
			this->suspended = true;

		this->stopMotion();

		float x = touch->getLocation().x, y = touch->getLocation().y;

		this->xTouch0 = x;
		this->yTouch0 = y;
		this->xLast = x;
		this->yLast = y;
		this->lastDeltaScroll = 0;
		this->updateBounds();

		this->tMove = clock();
		this->tTouch0 = this->tMove;

		this->showScrollPad();

		return true;
	}

	void VScrollView::onTouchMoved(Touch *touch, Event *event)
	{
		CCASSERT(content != nullptr, "NullReferenceException: content node of scrollview is null.");

		float x = touch->getLocation().x, y = touch->getLocation().y;

		if (this->tTouch0 == 0)
		{
			this->yLast = y;
			this->xTouch0 = x;
			this->yTouch0 = y;
			this->tTouch0 = clock();
		}

		float deltaScroll = y - this->yLast;
		float v = this->content->getPositionY() + deltaScroll;

		if (v < this->lowerBound)
			v = this->lowerBound;
		else if (v > this->upperBound)
			v = this->upperBound;

		this->content->setPositionY(v);

		// direction changed
		if (deltaScroll * this->lastDeltaScroll <= 0)
			this->yTouch0 = y;

		this->xLast = x;
		this->yLast = y;
		this->lastDeltaScroll = deltaScroll;

		this->tMove = clock();

		this->updateScrollPad(v);
	}

	void VScrollView::onTouchEnded(Touch *touch, Event *event)
	{
		float x = touch->getLocation().x, y = touch->getLocation().y;

		float deltaScroll = y - this->yTouch0;
		float deltaX = x - this->xTouch0;

		if (fabsf(deltaScroll) < 10)
		{
			if (fabs(deltaX) < 10)
			{
				this->stopMotion();
				if (!this->suspended)
					return;

				this->suspended = false;
			}
			else
			{
				this->hideScrollPad();
			}
		}
		else
		{
			clock_t t = clock();
			if ((float)(t - this->tMove) / CLOCKS_PER_SEC < 0.1f)
			{
				float deltaT = t - this->tTouch0;
				this->startMotion(2 * deltaScroll / ((float)deltaT / CLOCKS_PER_SEC));
				this->tTouch0 = 0;
			}
		}
	}

	void VScrollView::showScrollPad(void)
	{
		if (!this->scrollPad)
			return;

		this->scrollPad->stopAllActions();
		this->scrollPad->setOpacity(255);
	}

	void VScrollView::hideScrollPad(void)
	{
		if (!this->scrollPad)
			return;

		if (!this->scrollPadConsistent)
		{
			this->scrollPad->setOpacity(255);
			this->scrollPad->runAction(CCFadeOut::create(0.5f));
		}
	}

	void VScrollView::updateScrollPad(float y)
	{
		if (!this->scrollPad)
			return;

		float ratio = y / (this->upperBound - this->lowerBound);
		this->scrollPad->setPositionY(-ratio * (this->getContentSize().height - this->scrollPad->getContentSize().height));
	}

	Node * VScrollView::InflaterDelegate::createFromData(UIInflater *inflater, const ValueMap &data)
	{
		float viewWidth = 0, viewHeight = 0;
		Node *content = nullptr;

		viewWidth = ValueMapUtils::tryGetFloat("viewwidth", data);
		viewHeight = ValueMapUtils::tryGetFloat("viewheight", data);

		auto view = VScrollView::create(Size(viewWidth, viewHeight), nullptr);

		{
			auto it = data.find("content");
			if (it != data.end() && it->second.getType() == cocos2d::Value::Type::MAP)
			{
				UIInflater contentInflater(nullptr, inflater->getContainer());
				contentInflater.setScrollView(view);
				content = contentInflater.inflateNode(it->second.asValueMap());
			}
		}

		if (content)
			view->setContentNode(content);

		inflater->initElementProperties(view, data);
		inflater->initNodeProperties(view, data);
		inflater->initTouchProtocolProperties(view, data);

		return view;
	}

}
