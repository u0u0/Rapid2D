LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)
LOCAL_MODULE := static_png
LOCAL_SRC_FILES := $(TARGET_ARCH_ABI)/libpng.a
LOCAL_STATIC_LIBRARIES += static_z
include $(PREBUILT_STATIC_LIBRARY)
