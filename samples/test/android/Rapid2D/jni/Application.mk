APP_ABI := armeabi
APP_STL := gnustl_static
APP_PLATFORM := android-10
APP_CPPFLAGS := -std=c++11 -fsigned-char

ifeq ($(NDK_DEBUG),1)
  APP_CPPFLAGS += -DRD_DEBUG
endif
