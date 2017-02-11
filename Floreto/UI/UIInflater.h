#ifndef __FLORETO_UIINFLATER_H__
#define __FLORETO_UIINFLATER_H__

#include "../Common.h"
#include "TouchProtocol.h"
#include "ScrollViewProtocol.h"
#include "UIContainerProtocol.h"

#include <functional>
#include <unordered_map>

namespace Floreto
{
	class UIInflater
	{
	public:
		typedef std::function<cocos2d::Node * (const cocos2d::ValueMap &data)> ElementCreatorDelegate;
		
		enum class DuplicateTouchGroupPolicy
		{
			// Ignore the new group.
			Ignore,
			// Overwrite the old group.
			Overwrite,
			// Overwrite the old group and copy all touch elements to the new group.
			OverwriteAndCopy
		};

	protected:
		std::unordered_map<std::string, ElementCreatorDelegate> elementCreators;
		std::unordered_map<std::string, TouchGroup *> touchGroups;

		cocos2d::Node *node;
		UIContainerProtocol *container;
		ScrollViewProtocol *scrollView;

	public:
		UIInflater(cocos2d::Node *node, UIContainerProtocol *container);
		UIInflater(const UIInflater &src);
		virtual ~UIInflater(void);

		virtual inline cocos2d::Node * getContainerNode(void) const
		{
			return this->node;
		}

		virtual inline UIContainerProtocol * getContainer(void) const
		{
			return this->container;
		}

		virtual inline ScrollViewProtocol * getScrollView(void) const
		{
			return this->scrollView;
		}

		virtual inline void setScrollView(ScrollViewProtocol *scrollView)
		{
			this->scrollView = scrollView;
		}

	public:
		virtual bool addElementCreatorDelegate(const std::string &typeName, const ElementCreatorDelegate &delegate, bool forceAdd = false);
		virtual const ElementCreatorDelegate * getElementCreatorDelegate(const std::string &typeName);

		virtual void addTouchGroup(const std::string &name, TouchGroup *group, DuplicateTouchGroupPolicy policy = DuplicateTouchGroupPolicy::Ignore);
		virtual void addTouchGroup(TouchGroup *group, DuplicateTouchGroupPolicy policy = DuplicateTouchGroupPolicy::Ignore);
		virtual TouchGroup * getTouchGroup(const std::string &groupName);

		virtual bool inflate(const std::string &fileName);
		virtual bool inflate(const cocos2d::ValueMap &data);
		
		virtual cocos2d::Node * inflateNode(const cocos2d::ValueMap &data);

	protected:
		virtual void checkInflatingElements(cocos2d::Node *node, const cocos2d::ValueMap &data);
		virtual bool inflateElements(cocos2d::Node *node, const cocos2d::ValueVector &elements);

	public:
		virtual void initElementProperties(UIElementProtocol *element, const cocos2d::ValueMap &data);
		virtual void initNodeProperties(cocos2d::Node *node, const cocos2d::ValueMap &data);
		virtual void initTouchProtocolProperties(TouchProtocol *protocol, const cocos2d::ValueMap &data);
		virtual void initScrollViewAwareness(ScrollViewAwareProtocol *protocol, const cocos2d::ValueMap &data);

		static cocos2d::Color4B getColorFromCode(const std::string &colorCodeString);
		static void adjustNodeSize(cocos2d::Node *node, float horizontalPadding = 0, float verticalPadding = 0);
	};

	class UIInflaterDelegate
	{
	public:
		virtual cocos2d::Node * createFromData(UIInflater *inflater, const cocos2d::ValueMap &data) = 0;
	};

#define DeclareInflaterDelegate(__nodeclass__, __inflaterclass__) \
public: \
	class __inflaterclass__ : public Floreto::UIInflaterDelegate \
	{ \
	public: \
		virtual cocos2d::Node * createFromData(Floreto::UIInflater *inflater, const cocos2d::ValueMap &data) override; \
	public: \
		static __nodeclass__ * inflate(Floreto::UIInflater *inflater, const cocos2d::ValueMap &data) \
		{ \
			static __inflaterclass__ delegate = __inflaterclass__(); \
			return static_cast<__nodeclass__ *>(delegate.createFromData(inflater, data)); \
		} \
	}
}

#endif//__FLORETO_UIINFLATER_H__
