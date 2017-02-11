#ifndef __FLORETO_TOUCHPHASE_H__
#define __FLORETO_TOUCHPHASE_H__

namespace Floreto
{
	enum class TouchPhase
	{
		Began,
		Moved,
		Ended,
		Cancelled,
		Dismissed
	};
}

#endif//__FLORETO_TOUCHPHASE_H__
