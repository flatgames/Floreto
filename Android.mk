LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := floreto_static

LOCAL_MODULE_FILENAME := libfloreto

LOCAL_SRC_FILES := \
	Floreto\UI\Button.cpp \
	Floreto\UI\NinePatchSprite.cpp \
  Floreto\UI\Slider.cpp \
	Floreto\UI\TiledButton.cpp \
	Floreto\UI\ToggleButton.cpp \
	Floreto\UI\UIInflater.cpp \
	Floreto\UI\UIUtils.cpp \
	Floreto\UI\VisibleView.cpp \
	Floreto\UI\HPagedView.cpp \
	Floreto\UI\VScrollView.cpp \
	Floreto\Utils\BackKeyUtils.cpp \
	Floreto\Utils\DataUtils.cpp \
	Floreto\Utils\JsonUtils.cpp

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/..

LOCAL_C_INCLUDES := \
	$(LOCAL_PATH)/../cocos2d \
	$(LOCAL_PATH)/../cocos2d/cocos \
	$(LOCAL_PATH)/../cocos2d/cocos/platform/android
                                 
include $(BUILD_STATIC_LIBRARY)
