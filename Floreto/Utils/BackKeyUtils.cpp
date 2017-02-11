#include "BackKeyUtils.h"

using namespace cocos2d;

namespace Floreto
{
	static BackKeyUtils *instance = nullptr;

	BackKeyUtils * BackKeyUtils::getInstance(void)
	{
		if (!instance)
		{
			instance = new BackKeyUtils();
			instance->init();
			instance->retain();
		}

		return instance;
	}

	BackKeyUtils::BackKeyUtils(void)
	{
	}

	BackKeyUtils::~BackKeyUtils(void)
	{
	}

	void BackKeyUtils::attachToNotification(void)
	{
		auto notif = Director::getInstance()->getNotificationNode();
		if (!notif)
		{
			notif = Node::create();
			Director::getInstance()->setNotificationNode(notif);

			notif->onEnter();
		}

		notif->addChild(this);

		EventListenerKeyboard *keyboardListener = EventListenerKeyboard::create();
		keyboardListener->onKeyReleased = [this](EventKeyboard::KeyCode keyCode, Event *)
		{
			if (keyCode == EventKeyboard::KeyCode::KEY_ESCAPE
				|| keyCode == EventKeyboard::KeyCode::KEY_BACK
				|| keyCode == EventKeyboard::KeyCode::KEY_BACKSPACE)
			{
				if (this->_listeners.size() > 0)
				{
					const auto &top = this->_listeners.back();
					top.callback();
				}
			}
		};
		this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(keyboardListener, this);
	}

	void BackKeyUtils::pushListener(const Callback &callback)
	{
		this->_listeners.push_back(Listener(nullptr, callback));
	}

	void BackKeyUtils::pushListener(void *tag, const Callback &callback)
	{
		this->_listeners.push_back(Listener(tag, callback));
	}

	void BackKeyUtils::popListener(void *tag)
	{
		auto &listeners = this->_listeners;
		for (int i = listeners.size() - 1; i >= 0; --i)
		{
			const auto &listener = listeners[i];
			if (listener.tag == tag)
			{
				this->_listeners.erase(listeners.begin() + i);
				return;
			}
		}
	}
}
