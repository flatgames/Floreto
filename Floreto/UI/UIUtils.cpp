#include "UIUtils.h"

using namespace cocos2d;

namespace Floreto
{
	void UIUtils::calculateContentNodeSize(Node *contentNode, float xPadding, float yPadding, bool ignoreInvisibleChildren)
	{
		float xMin = 0, xMax = 0;
		float yMin = 0, yMax = 0;

		const auto &arr = contentNode->getChildren();
		for (int i = 0, c = arr.size(); i < c; ++i)
		{
			Node *child = arr.at(i);
			if (ignoreInvisibleChildren && !child->isVisible()) continue;

			Rect bounds = child->getBoundingBox();
			if (bounds.getMinX() < xMin)
				xMin = bounds.getMinX();
			if (bounds.getMaxX() > xMax)
				xMax = bounds.getMaxX();
			if (bounds.getMinY() < yMin)
				yMin = bounds.getMinY();
			if (bounds.getMaxY() > yMax)
				yMax = bounds.getMaxY();
		}

		contentNode->setContentSize(Size(xMax - xMin + xPadding * 2, yMax - yMin + yPadding * 2));

		for (int i = 0, c = arr.size(); i < c; ++i)
		{
			Node *child = arr.at(i);
			if (ignoreInvisibleChildren && !child->isVisible()) continue;
			child->setPosition(
				child->getPositionX() - xMin + xPadding,
				child->getPositionY() - yMin + yPadding);
		}
	}

	Sprite * UIUtils::createRectangle(float width, float height, Color3B color)
	{
		auto sprite = Sprite::create();
		sprite->setTextureRect(Rect(0, 0, width, height));
		sprite->setColor(color);

		return sprite;
	}

	Color3B UIUtils::lightenColor(const Color3B &color, float percent)
	{
		int r = color.r + color.r * percent;
		int g = color.g + color.g * percent;
		int b = color.b + color.b * percent;

		if (r > 255)
			r = 255;
		if (g > 255)
			g = 255;
		if (b > 255)
			b = 255;

		return Color3B((GLubyte)r, (GLubyte)g, (GLubyte)b);
	}

	Color3B UIUtils::darkenColor(const Color3B &color, float percent)
	{
		int r = color.r - color.r * percent;
		int g = color.g - color.g * percent;
		int b = color.b - color.b * percent;

		if (r < 0)
			r = 0;
		if (g < 0)
			g = 0;
		if (b < 0)
			b = 0;

		return Color3B((GLubyte)r, (GLubyte)g, (GLubyte)b);
	}

}
