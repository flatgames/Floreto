#ifndef __FLORETO_VISIBLEVIEW_H__
#define __FLORETO_VISIBLEVIEW_H__

#include "../Common.h"

namespace Floreto
{
	class VisibleView
	{
	public:
		// Resolution policy.
		enum class Policy
		{
			// Keep design width and leave design height expanded or clipped.
			// Design width keeps as provided. Design height may be bigger or smaller than provided.
			FixedWidth,
			// Keep design height and leave design width expanded or clipped.
			// Design height keeps as provided. Design width may be bigged or smaller than provided.
			FixedHeight,
			// Leave design width or design height clipped if needed.
			// Either design width or design height may be bigger or smaller than provided. No black border appears.
			NoBorder,
			// Keep design width and design height. Redundant area is not usable.
			// Both design width and design height keep as provided. Black borders may appear.
			ShowAll,
			// Leave design width or design height expanded. Redundant area is usable.
			// Either design width or design height may be bigger than provided. Black borders may appear.
			ShowAllExtended
		};

	public:
		static void init(float designWidth, float designHeight, Policy policy);

	private:
		static float _width;
		static float _height;
		static float _x;
		static float _y;
		static float _centerX;
		static float _centerY;
		static float _onePixel;

	public:
		inline static float width(void)
		{
			return _width;
		}

		inline static float height(void)
		{
			return _height;
		}

		inline static float x(void)
		{
			return _x;
		}

		inline static float y(void)
		{
			return _y;
		}

		inline static float cx(void)
		{
			return _centerX;
		}

		inline static float cy(void)
		{
			return _centerY;
		}

		inline static float onePixel(void)
		{
			return _onePixel;
		}
	};
}

#endif//__FLORETO_VISIBLEVIEW_H__
