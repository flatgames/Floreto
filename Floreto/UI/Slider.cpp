#include "Slider.h"

using namespace cocos2d;

namespace Floreto
{

	Slider * Slider::createWithSpriteFrame(Sprite *bgSprite, Sprite *fillSprite, Sprite *thumbSprite,
		ValueChangedDelegate onValueChanged)
	{
		Slider *slider = new Slider();
		if (slider && slider->init(bgSprite, fillSprite, thumbSprite, onValueChanged))
		{
			CCAssert(bgSprite || fillSprite, "Either background or filling should be given.");
			if (bgSprite)
				slider->setContentSize(bgSprite->getContentSize());
			else if (fillSprite)
				slider->setContentSize(fillSprite->getContentSize());

			slider->autorelease();
			return slider;
		}

		CC_SAFE_DELETE(slider);
		return nullptr;
	}

	Slider * Slider::createWithFile(const std::string &bgFile, const std::string &fillFile, const std::string &thumbFile,
		ValueChangedDelegate onValueChanged)
	{
		Sprite *bgSprite = nullptr, *fillSprite = nullptr, *thumbSprite = nullptr;
		if (bgFile.size())
			bgSprite = Sprite::create(bgFile);
		if (fillFile.size())
			fillSprite = Sprite::create(fillFile);
		if (thumbFile.size())
			thumbSprite = Sprite::create(thumbFile);

		return Slider::createWithSpriteFrame(bgSprite, fillSprite, thumbSprite,
			onValueChanged);
	}

	Slider * Slider::createWithSpriteFrameName(const std::string &bgSpriteName, const std::string &fillSpriteName, const std::string &thumbSpriteName,
		ValueChangedDelegate onValueChanged)
	{
		Sprite *bgSprite = nullptr, *fillSprite = nullptr, *thumbSprite = nullptr;
		if (bgSpriteName.size())
			bgSprite = Sprite::createWithSpriteFrameName(bgSpriteName);
		if (fillSpriteName.size())
			fillSprite = Sprite::createWithSpriteFrameName(fillSpriteName);
		if (thumbSpriteName.size())
			thumbSprite = Sprite::createWithSpriteFrameName(thumbSpriteName);

		return Slider::createWithSpriteFrame(bgSprite, fillSprite, thumbSprite,
			onValueChanged);
	}

	Slider::Slider(void)
		: onValueChanged(nullptr), orientation(Orientation::Horizontal),
		bgSprite(nullptr), fillSprite(nullptr), thumbSprite(nullptr),
		minValue(0), maxValue(0), valueSpan(0), currentValue(0), lastValue(0), xyLast(0), isActive(false)
	{
	}

	Slider::~Slider(void)
	{
		CC_SAFE_RELEASE_NULL(bgSprite);
		CC_SAFE_RELEASE_NULL(fillSprite);
		CC_SAFE_RELEASE_NULL(thumbSprite);
	}

	bool Slider::init(Sprite *bgSprite, Sprite *fillSprite, Sprite *thumbSprite, ValueChangedDelegate onValueChanged)
	{
		Button::SingleTouchDelegate touchDelegate = CC_CALLBACK_3(Slider::onTouch, this);
		if (!Super::init(nullptr, touchDelegate, nullptr, false, false))
			return false;

		if (bgSprite)
		{
			this->addChild(bgSprite, 0);
			this->bgSprite = bgSprite;
			CC_SAFE_RETAIN(bgSprite);
			bgSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			bgSprite->setPositionY(bgSprite->getContentSize().height * 0.5f);
		}

		if (fillSprite)
		{
			this->addChild(fillSprite, 1);
			this->fillSprite = fillSprite;
			CC_SAFE_RETAIN(fillSprite);
			fillSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			fillSprite->setPositionY(bgSprite->getPositionY());
		}

		if (thumbSprite)
		{
			this->addChild(thumbSprite, 2);
			this->thumbSprite = thumbSprite;
			CC_SAFE_RETAIN(thumbSprite);
			thumbSprite->setPositionY(bgSprite->getPositionY());
		}

		this->onValueChanged = onValueChanged;

		return true;
	}

	void Slider::setValues(float minValue, float maxValue, float currentValue)
	{
		this->minValue = minValue;
		this->maxValue = maxValue;
		this->valueSpan = maxValue - minValue;
		this->currentValue = currentValue;

		this->updateAppearance();
	}

	void Slider::setCurrentValue(float value)
	{
		if (value >= this->minValue && value <= this->maxValue)
		{
			this->lastValue = this->currentValue;
			this->currentValue = value;
			this->updateAppearance();
		}
	}

	Slider::Orientation Slider::getOrientation(void)
	{
		return this->orientation;
	}

	void Slider::setOrientation(Orientation orientation)
	{
		if (this->orientation != orientation)
		{
			if (orientation == Orientation::Horizontal)
			{
				if (this->fillSprite)
				{
					this->fillSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
					this->fillSprite->setPosition(0, this->getContentSize().height / 2);
				}

				if (this->thumbSprite)
					this->thumbSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_LEFT);
			}
			else
			{
				if (this->fillSprite)
				{
					this->fillSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
					this->fillSprite->setPosition(this->getContentSize().width / 2, 0);
				}

				if (this->thumbSprite)
					this->thumbSprite->setAnchorPoint(Vec2::ANCHOR_MIDDLE_BOTTOM);
			}

			float currentValue = this->currentValue;
			this->setCurrentValue(this->maxValue);

			this->orientation = orientation;
			this->setCurrentValue(currentValue);
		}
	}

	void Slider::updateAppearance(void)
	{
		float ratio = (this->currentValue - this->minValue) / this->valueSpan;
		Rect textureRect = this->fillSprite->getTextureRect();

		if (this->orientation == Orientation::Horizontal)
		{
			float x = ratio * this->getContentSize().width;
			textureRect.size.width = x;

			if (this->thumbSprite)
				this->thumbSprite->setPositionX(x);
		}
		else
		{
			float lastRatio = (this->lastValue - this->minValue) / this->valueSpan;
			float fullHeight = textureRect.size.height / lastRatio;
			float height = fullHeight * ratio;
			float y = fullHeight - height;

			textureRect.origin.y = textureRect.origin.y + (fullHeight * (lastRatio - ratio));
			textureRect.size.height = height;

			if (this->thumbSprite)
				this->thumbSprite->setPositionY(y);
		}
		
		this->fillSprite->setTextureRect(textureRect, this->fillSprite->isTextureRectRotated(), textureRect.size);
	}

	float Slider::getValueFromPosition(float pos)
	{
		float ratio;
		if (this->orientation == Orientation::Horizontal)
			ratio = pos / this->getContentSize().width;
		else
			ratio = pos / this->getContentSize().height;
		
		return minValue + valueSpan * ratio;
	}

	bool Slider::onTouch(Button *button, cocos2d::Touch *touch, TouchPhase phase)
	{
		float x = touch->getLocation().x;
		float y = touch->getLocation().y;
		Vec2 p = this->convertToNodeSpace(touch->getLocation());

		switch (phase)
		{
			case Floreto::TouchPhase::Began:
			{
				if (this->bgSprite->getBoundingBox().containsPoint(p))
				{
					float value = this->getValueFromPosition(this->orientation == Orientation::Horizontal ? p.x : p.y);
					this->setCurrentValue(value);
					if (this->onValueChanged)
						this->onValueChanged(this, this->currentValue);

					this->xyLast = this->orientation == Orientation::Horizontal ? y : x;
					this->isActive = true;
					this->setIgnoringScrollView(true);
				}
				else
				{
					this->setIgnoringScrollView(false);
				}

				return true;
			}
			case Floreto::TouchPhase::Moved:
			{
				if (this->isActive)
				{
					float value = this->getValueFromPosition(this->orientation == Orientation::Horizontal ? p.x : p.y);
					this->setCurrentValue(value);
					if (this->onValueChanged)
						this->onValueChanged(this, this->currentValue);

					if (this->scrollView
						&& this->scrollView->getOrientation() == (this->orientation == Orientation::Horizontal ? ScrollViewProtocol::Orientation::Vertical : ScrollViewProtocol::Orientation::Horizontal)
						&& fabsf((this->orientation == Orientation::Horizontal ? y : x) - this->xyLast) >  (this->orientation == Orientation::Horizontal ? this->getContentSize().height : this->getContentSize().width))
						this->setIgnoringScrollView(false);
				}
				break;
			}
			case Floreto::TouchPhase::Ended:
			case Floreto::TouchPhase::Cancelled:
			{
				this->isActive = false;
				break;
			}
			case Floreto::TouchPhase::Dismissed:
			{
				this->isActive = false;
				break;
			}
		}

		return false;
	}

	void Slider::onTouchEffect(TouchPhase phase, bool autoEnd)
	{
		Super::onTouchEffect(phase, autoEnd);
	}

}
