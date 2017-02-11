#ifndef __FLORETO_BACKKEYUTILS_H__
#define __FLORETO_BACKKEYUTILS_H__

#include "../Common.h"

namespace Floreto
{
	class BackKeyUtils : public cocos2d::Node
	{
	public:
		typedef std::function<bool(void)> Callback;
		struct Listener
		{
			void *tag;
			Callback callback;

			Listener(void)
				: tag(nullptr), callback(nullptr)
			{
			}

			Listener(void *tag, const Callback &callback)
				: tag(tag), callback(callback)
			{
			}
		};

	public:
		static BackKeyUtils * getInstance(void);

	protected:
		std::vector<Listener> _listeners;

	public:
		BackKeyUtils(void);
		~BackKeyUtils(void);

	public:
		void attachToNotification(void);
		void pushListener(const Callback &callback);
		void pushListener(void *tag, const Callback &callback);
		void popListener(void *tag);
	};
}

#endif//__FLORETO_BACKKEYUTILS_H__
