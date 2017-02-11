#include "UIInflater.h"
#include "../Utils/ClassModel.h"
#include "../Utils/ValueMapUtils.h"
#include "../Utils/JsonUtils.h"

#include "Button.h"
#include "TiledButton.h"
#include "ToggleButton.h"
#include "VScrollView.h"

#include <cstdlib> //for strtoul

using namespace cocos2d;

namespace Floreto
{

	UIInflater::UIInflater(const UIInflater &src)
		: node(src.node), container(src.container), scrollView(nullptr)
	{
		this->elementCreators = src.elementCreators;
	}

	UIInflater::UIInflater(Node *node, UIContainerProtocol *container)
		: node(node), container(container), scrollView(nullptr)
	{
		elementCreators["Node"] = [this](const ValueMap &data) -> Node *
		{
			auto node = Node::create();
			this->initNodeProperties(node, data);
			return node;
		};

		elementCreators["Label"] = [this](const ValueMap &data) -> Node *
		{
			Label *label = nullptr;
			std::string kind = ValueMapUtils::tryGetString("kind", data);
			std::string text = ValueMapUtils::tryGetString("text", data);
			std::string fontName = ValueMapUtils::tryGetString("fontName", data);
			std::string halign_ = ValueMapUtils::tryGetString("halign", data, "left");

			TextHAlignment halign = TextHAlignment::LEFT;

			if (!halign_.compare("left"))
				halign = TextHAlignment::LEFT;
			else if (!halign_.compare("right"))
				halign = TextHAlignment::RIGHT;
			else if (!halign_.compare("center"))
				halign = TextHAlignment::CENTER;

			if (!kind.compare("ttf"))
			{
				float fontSize = Floreto::ValueMapUtils::tryGetFloat("fontSize", data, 10);

				float dw = ValueMapUtils::tryGetFloat("dw", data, 0);
				float dh = ValueMapUtils::tryGetFloat("dh", data, 0);
				Size dimens = Size(dw, dh);

				std::string valign_ = ValueMapUtils::tryGetString("valign", data, "top");
				TextVAlignment valign = TextVAlignment::TOP;
				if (!valign_.compare("top"))
					valign = TextVAlignment::TOP;
				else if (!valign_.compare("bottom"))
					valign = TextVAlignment::BOTTOM;
				else if (!valign_.compare("center"))
					valign = TextVAlignment::CENTER;

				label = Label::createWithTTF(text, fontName, fontSize, dimens, halign, valign);

				auto it = data.find("effect");
				if (it != data.end() && it->second.getType() == Value::Type::MAP)
				{
					const auto &fx = it->second.asValueMap();
					const auto &type = ValueMapUtils::tryGetString("type", fx);
					if (!type.compare("outline"))
					{
						Color4B color = UIInflater::getColorFromCode(ValueMapUtils::tryGetString("color", fx, "0xff000000"));
						int width = ValueMapUtils::tryGetInt("width", fx, -1);

						label->enableOutline(color, width);
					}
					else if (!type.compare("shadow"))
					{
						Color4B color = UIInflater::getColorFromCode(ValueMapUtils::tryGetString("color", fx, "0xff000000"));
						float xOffset = ValueMapUtils::tryGetFloat("xoffset", fx, 2);
						float yOffset = ValueMapUtils::tryGetFloat("yoffset", fx, -2);
						int radius = ValueMapUtils::tryGetInt("radius", fx, 0);
						
						label->enableShadow(color, Size(xOffset, yOffset), radius);
					}
					else if (!type.compare("glow"))
					{
						Color4B color = UIInflater::getColorFromCode(ValueMapUtils::tryGetString("color", fx, "0xff000000"));
						label->enableGlow(color);
					}
				}
			}
			else if (!kind.compare("fnt"))
			{
				label = Label::createWithBMFont(fontName, text, halign);
			}

			if (!label)
				return nullptr;

			this->initNodeProperties(label, data);

			return label;
		};

		elementCreators["Sprite"] = [this](const ValueMap &data) -> Node *
		{
			cocos2d::ValueMap::const_iterator it;
			std::string fileName, frameName;
			bool popout = false, multitouch = false;

			if ((it = data.find("fileName")) != data.end())
				fileName = it->second.asString();
			else if ((it = data.find("frameName")) != data.end())
				frameName = it->second.asString();
			else
				return nullptr;

			Sprite *sprite = nullptr;
			if (fileName.length())
				sprite = Sprite::create(fileName);
			else if (frameName.length())
				sprite = Sprite::createWithSpriteFrameName(frameName);
			else
				return nullptr;

			this->initNodeProperties(sprite, data);

			return sprite;
		};

		elementCreators["Button"] = [this](const ValueMap &data)
		{
			auto button = Button::InflaterDelegate::inflate(this, data);
			return button;
		};

		elementCreators["TiledButton"] = [this](const ValueMap &data)
		{
			auto button = TiledButton::InflaterDelegate::inflate(this, data);
			return button;
		};

		elementCreators["ToggleButton"] = [this](const ValueMap &data)
		{
			auto button = ToggleButton::InflaterDelegate::inflate(this, data);
			return button;
		};

		elementCreators["VScrollView"] = [this](const ValueMap &data)
		{
			auto view = VScrollView::InflaterDelegate::inflate(this, data);
			
			if (view->getContentNode())
				view->refresh();

			return view;
		};
	}

	UIInflater::~UIInflater(void)
	{
	}

	bool UIInflater::addElementCreatorDelegate(const std::string &typeName, const ElementCreatorDelegate &delegate, bool forceAdd)
	{
		auto itCreator = this->elementCreators.find(typeName);
		if (itCreator != this->elementCreators.end())
		{
			if (forceAdd)
				this->elementCreators[typeName] = delegate;

			return false;
		}
		else
		{
			this->elementCreators[typeName] = delegate;
			return true;
		}
	}

	const UIInflater::ElementCreatorDelegate * UIInflater::getElementCreatorDelegate(const std::string &typeName)
	{
		auto itCreator = this->elementCreators.find(typeName);
		if (itCreator != this->elementCreators.end())
			return &itCreator->second;
		else
			return nullptr;
	}

	void UIInflater::addTouchGroup(const std::string &name, TouchGroup *group, DuplicateTouchGroupPolicy policy)
	{
		group->setName(name);
		this->addTouchGroup(group, policy);
	}

	void UIInflater::addTouchGroup(TouchGroup *group, DuplicateTouchGroupPolicy policy)
	{
		auto it = this->touchGroups.find(group->getName());
		if (it != this->touchGroups.end())
		{
			switch (policy)
			{
				case DuplicateTouchGroupPolicy::Ignore:
				{
					// Do nothing.
					break;
				}
				case DuplicateTouchGroupPolicy::Overwrite:
				{
					this->touchGroups[group->getName()] = group;
					break;
				}
				case DuplicateTouchGroupPolicy::OverwriteAndCopy:
				{
					auto oldGroup = it->second;
					group->acceptElements(oldGroup);
					this->touchGroups[group->getName()] = group;
					break;
				}
			}
		}
		else
		{
			this->touchGroups[group->getName()] = group;
		}
	}

	TouchGroup * UIInflater::getTouchGroup(const std::string &groupName)
	{
		auto it = this->touchGroups.find(groupName);
		if (it != this->touchGroups.end())
			return it->second;
		else
			return nullptr;
	}

	bool UIInflater::inflate(const std::string &fileName)
	{
		std::string s = FileUtils::getInstance()->getStringFromFile(fileName);
		const Value &value = JsonUtils::getValueFromJson(s.c_str());
		if (value.getType() == Value::Type::MAP)
			return this->inflate(value.asValueMap());
		else
			return false;
	}

	bool UIInflater::inflate(const ValueMap &data)
	{
		if (data.find("elements") == data.end())
			return false;

		const auto &value = data.at("elements");
		if (value.getType() != Value::Type::VECTOR)
			return false;

		const auto &list = value.asValueVector();
		return this->inflateElements(this->node, list);
	}

	Node * UIInflater::inflateNode(const cocos2d::ValueMap &data)
	{
		auto it = this->elementCreators.find("Node");
		if (it != this->elementCreators.end())
			return it->second(data);
		else
			return nullptr;
	}

	void UIInflater::checkInflatingElements(Node *node, const ValueMap &data)
	{
		auto it = data.find("elements");
		if (it != data.end())
		{
			const auto &v = it->second;
			if (v.getType() == Value::Type::VECTOR)
			{
				const auto &elements = v.asValueVector();
				this->inflateElements(node, elements);
			}
		}
	}

	bool UIInflater::inflateElements(Node *node, const ValueVector &elements)
	{
		for (auto it = elements.begin(), itEnd = elements.end(); it != itEnd; ++it)
		{
			const auto &details = it->asValueMap();
			const auto &type = details.at("type").asString();

			auto itCreator = this->elementCreators.find(type);
			if (itCreator != this->elementCreators.end())
			{
				auto elementAsNode = itCreator->second(details);
				auto element = dynamic_cast<UIElementProtocol *>(elementAsNode);
				node->addChild(elementAsNode);
				if (element)
					this->container->addElement(element->getName(), element);
			}
			else
			{
				CCLOG("Warning: found element of unknown type '%s', ignored.", type.c_str());
			}
		}

		return true;
	}

	void UIInflater::initElementProperties(UIElementProtocol *element, const ValueMap &data)
	{
		const auto &name = ValueMapUtils::tryGetString("name", data);
		element->setName(name);
	}

	void UIInflater::initNodeProperties(Node *node, const ValueMap &data)
	{
		float x = 0, y = 0, ax = 0.5f, ay = 0.5f;
		float rx = 0, ry = 0;
		float sx = 0, sy = 0;
		int tag, zOrder;

		x = ValueMapUtils::tryGetFloat("x", data);
		y = ValueMapUtils::tryGetFloat("y", data);
		ax = ValueMapUtils::tryGetFloat("ax", data, 0.5f);
		ay = ValueMapUtils::tryGetFloat("ay", data, 0.5f);
		rx = ValueMapUtils::tryGetFloat("rx", data);
		ry = ValueMapUtils::tryGetFloat("ry", data);
		sx = ValueMapUtils::tryGetFloat("sx", data, 1);
		sy = ValueMapUtils::tryGetFloat("sy", data, 1);
		tag = ValueMapUtils::tryGetInt("tag", data);
		zOrder = ValueMapUtils::tryGetInt("zorder", data);

		{
			float w = 0, h = 0;
			auto itWidth = data.find("width");
			auto itHeight = data.find("height");
			
			if (itWidth != data.end()
				&& (itWidth->second.getType() == Value::Type::INTEGER || itWidth->second.getType() == Value::Type::FLOAT))
				w = itWidth->second.asFloat();
			else
				w = node->getContentSize().width;

			if (itHeight != data.end()
				&& (itHeight->second.getType() == Value::Type::INTEGER || itHeight->second.getType() == Value::Type::FLOAT))
				h = itHeight->second.asFloat();
			else
				h = node->getContentSize().height;

			node->setContentSize(Size(w, h));
		}

		node->setScaleX(sx);
		node->setScaleY(sy);

		node->setTag(tag);
		node->setLocalZOrder(zOrder);
		node->setAnchorPoint(cocos2d::Point(ax, ay));

		const auto &parentSize = this->node ? this->node->getContentSize() : cocos2d::Director::getInstance()->getWinSize();
		node->setPosition(x + parentSize.width * rx, y + parentSize.height * ry);

		this->checkInflatingElements(node, data);

		bool autosize = ValueMapUtils::tryGetBool("autosize", data, false);
		if (autosize)
		{
			float hpadding = ValueMapUtils::tryGetFloat("hpadding", data);
			float vpadding = ValueMapUtils::tryGetFloat("vpadding", data);
			UIInflater::adjustNodeSize(node, hpadding, vpadding);
		}

		{
			auto it = data.find("color");
			if (it != data.end() && it->second.getType() == Value::Type::STRING)
			{
				Color4B color = UIInflater::getColorFromCode(it->second.asString());
				node->setColor(Color3B(color));
			}
		}

		int opacity = ValueMapUtils::tryGetInt("opacity", data, 255);
		if (opacity > 255)
			opacity = 255;
		else if (opacity < 0)
			opacity = 0;
		node->setOpacity(opacity);
	}

	void UIInflater::initTouchProtocolProperties(TouchProtocol *protocol, const cocos2d::ValueMap &data)
	{
		const auto &touchGroupName = ValueMapUtils::tryGetString("touchgroup", data);
		if (touchGroupName.length())
		{
			auto group = this->getTouchGroup(touchGroupName);
			if (group)
				group->addElement(protocol);
		}
	}

	void UIInflater::initScrollViewAwareness(ScrollViewAwareProtocol *protocol, const cocos2d::ValueMap &data)
	{
		auto scrollView = this->getScrollView();
		if (scrollView)
		{
			bool ignoreScrollView = ValueMapUtils::tryGetBool("ignorescrollview", data);
			protocol->setScrollView(scrollView);
			protocol->setIgnoringScrollView(ignoreScrollView);
		}
	}

	Color4B UIInflater::getColorFromCode(const std::string &colorCodeString)
	{
		unsigned long code = strtoul(colorCodeString.c_str(), nullptr, 16); //std::stoul(colorCodeString, nullptr, 16);
		Color4B color = Color4B(
			(code & 0x00FF0000) >> 16,
			(code & 0x0000FF00) >> 8,
			(code & 0x000000FF),
			(code & 0xFF000000) >> 24);

		return color;
	}

	void UIInflater::adjustNodeSize(Node *node, float horizontalPadding, float verticalPadding)
	{
		float xMin = 0, xMax = 0;
		float yMin = 0, yMax = 0;

		const auto &children = node->getChildren();
		for (auto it = children.begin(), itEnd = children.end(); it != itEnd; ++it)
		{
			const auto &bounds = (*it)->getBoundingBox();
			if (bounds.getMinX() < xMin)
				xMin = bounds.getMinX();
			if (bounds.getMaxX() > xMax)
				xMax = bounds.getMaxX();
			if (bounds.getMinY() < yMin)
				yMin = bounds.getMinY();
			if (bounds.getMaxY() > yMax)
				yMax = bounds.getMaxY();
		}

		xMin -= horizontalPadding;
		xMax += horizontalPadding;
		yMin -= verticalPadding;
		yMax += verticalPadding;

		node->setContentSize(Size(xMax - xMin, yMax - yMin));

		for (auto it = children.begin(), itEnd = children.end(); it != itEnd; ++it)
		{
			auto child = *it;
			child->setPosition(
				child->getPositionX() - xMin,
				child->getPositionY() - yMin);
		}
	}
}
