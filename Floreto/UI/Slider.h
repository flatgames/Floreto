#ifndef __FLORETO_SLIDER_H__
#define __FLORETO_SLIDER_H__

#include "../Common.h"
#include "Button.h"

namespace Floreto
{

	class Slider : public Button
	{
		typedef Button Super;

	public:
		typedef std::function<void(Slider *slider, float value)> ValueChangedDelegate;

		enum class Orientation
		{
			Horizontal,
			Vertical
		};

	protected:
		ValueChangedDelegate onValueChanged;
		Orientation orientation;

		Sprite *bgSprite;
		Sprite *fillSprite;
		Sprite *thumbSprite;

		float minValue;
		float maxValue;
		float valueSpan;
		float currentValue;
		float lastValue;

		float xyLast;
		bool isActive;

	protected:
		bool init(Sprite *bgSprite, Sprite *fillSprite, Sprite *thumbSprite, ValueChangedDelegate onValueChanged);

	public:
		static Slider * createWithSpriteFrame(Sprite *bgSprite, Sprite *fillSprite, Sprite *thumbSprite,
			ValueChangedDelegate onValueChanged);

		static Slider * createWithFile(const std::string &bgFile, const std::string &fillFile, const std::string &thumbFile,
			ValueChangedDelegate onValueChanged);

		static Slider * createWithSpriteFrameName(const std::string &bgSprite, const std::string &fillSprite, const std::string &thumbSprite,
			ValueChangedDelegate onValueChanged);

		Slider(void);
		virtual ~Slider(void);

		inline Sprite * getBackgroundSprite(void)
		{
			return this->bgSprite;
		}

		inline Sprite * getFillSprite(void)
		{
			return this->fillSprite;
		}

		inline Sprite * getThumbSprite(void)
		{
			return this->thumbSprite;
		}

		inline ValueChangedDelegate getOnValueChanged(void)
		{
			return this->onValueChanged;
		}

		inline void setOnValueChanged(ValueChangedDelegate onValueChanged)
		{
			this->onValueChanged = onValueChanged;
		}

		inline float getMinValue(void)
		{
			return this->minValue;
		}

		inline float getMaxValue(void)
		{
			return this->maxValue;
		}

		inline float getCurrentValue(void)
		{
			return this->currentValue;
		}

		void setValues(float minValue, float maxValue, float currentValue);
		void setCurrentValue(float value);

		Orientation getOrientation(void);
		void setOrientation(Orientation orientation);

	protected:
		void updateAppearance(void);
		float getValueFromPosition(float pos);

	protected:
		bool onTouch(Button *, cocos2d::Touch *, TouchPhase);
		virtual void onTouchEffect(TouchPhase phase, bool autoEnd = false) override;
	};

}

#endif//__FLORETO_SLIDER_H__
