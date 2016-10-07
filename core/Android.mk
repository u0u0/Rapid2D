LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE := static_Rapid2D

LOCAL_MODULE_FILENAME := libRapid2D

LOCAL_CFLAGS := -DRD_PLATFORM_ANDROID

LOCAL_C_INCLUDES := $(LOCAL_PATH) \
	$(LOCAL_PATH)/audio \
	$(LOCAL_PATH)/crypto \
	$(LOCAL_PATH)/math \
	$(LOCAL_PATH)/porting/inc \
	$(LOCAL_PATH)/shader \
	$(LOCAL_PATH)/sprite \
	$(LOCAL_PATH)/texture \
	$(LOCAL_PATH)/utils \
	$(LOCAL_PATH)/scripting/lua \
	$(LOCAL_PATH)/scripting/binding \
	$(LOCAL_PATH)/../thirdParty/png/inc \
	$(LOCAL_PATH)/../thirdParty/OpenAL/inc

# core
LOCAL_SRC_FILES := \
	Rapid2D.cpp \
	RDConfig.cpp \
	RDDirector.cpp \
	RDResolution.cpp
# crypto
LOCAL_SRC_FILES += \
	crypto/base64.c \
	crypto/md5.c \
	crypto/xxtea.c
# math
LOCAL_SRC_FILES += \
	math/RDkmMath.c \
	math/RDMath.c
# shader
LOCAL_SRC_FILES += \
	shader/RDIndexBuffer.cpp \
	shader/RDProgram.cpp \
	shader/RDProgramCache.cpp
# sprite
LOCAL_SRC_FILES += \
	sprite/RDSprite.cpp \
	sprite/RDDrawVertex.cpp
# texture
LOCAL_SRC_FILES += \
	texture/RDTexture.cpp \
	texture/RDTexturePng.cpp
# utils
LOCAL_SRC_FILES += \
	utils/RDData.cpp \
	utils/RDFileSystem.cpp \
	utils/RDLog.cpp
# porting
LOCAL_SRC_FILES += \
	porting/android/c2java.cpp \
	porting/android/RDPortingPrint.cpp \
	porting/android/RDPortingSystem.cpp \
	porting/android/RDRDPortingFile.cpp \
	porting/android/RDPortingTTF.cpp \
	porting/android/RDRDPortingMainLoop.cpp
# scripting/lua
LOCAL_SRC_FILES += \
	scripting/lua/lapi.c \
	scripting/lua/lauxlib.c \
	scripting/lua/lbaselib.c \
	scripting/lua/lbitlib.c \
	scripting/lua/lcode.c \
	scripting/lua/lcorolib.c \
	scripting/lua/lctype.c \
	scripting/lua/ldblib.c \
	scripting/lua/ldebug.c \
	scripting/lua/ldo.c \
	scripting/lua/ldump.c \
	scripting/lua/lfunc.c \
	scripting/lua/lgc.c \
	scripting/lua/linit.c \
	scripting/lua/liolib.c \
	scripting/lua/llex.c \
	scripting/lua/lmathlib.c \
	scripting/lua/lmem.c \
	scripting/lua/loadlib.c \
	scripting/lua/lobject.c \
	scripting/lua/lopcodes.c \
	scripting/lua/loslib.c \
	scripting/lua/lparser.c \
	scripting/lua/lstate.c \
	scripting/lua/lstring.c \
	scripting/lua/lstrlib.c \
	scripting/lua/ltable.c \
	scripting/lua/ltablib.c \
	scripting/lua/ltm.c \
	scripting/lua/lundump.c \
	scripting/lua/lutf8lib.c \
	scripting/lua/lvm.c \
	scripting/lua/lzio.c
# scripting/cjson
LOCAL_SRC_FILES += \
	scripting/cjson/fpconv.c \
	scripting/cjson/lua_cjson.c \
	scripting/cjson/strbuf.c
# scripting/binding
LOCAL_SRC_FILES += \
	scripting/binding/RDlaudio.cpp \
	scripting/binding/RDlcrypto.cpp \
	scripting/binding/RDldrawvertex.cpp \
	scripting/binding/RDlfilesystem.cpp \
	scripting/binding/RDllualoader.cpp \
	scripting/binding/RDlMat4.cpp \
	scripting/binding/RDlOpenGL.cpp \
	scripting/binding/RDlsprite.cpp \
	scripting/binding/RDlspriteframe.cpp \
	scripting/binding/RDltexture.cpp \
	scripting/binding/RDLua.cpp

# link for static library
LOCAL_STATIC_LIBRARIES := static_png static_Rapid2DAudio static_OpenAL
# module link depend library
LOCAL_EXPORT_LDLIBS := -lGLESv2 \
	-llog \
	-landroid \
	-lOpenSLES

include $(BUILD_STATIC_LIBRARY)

# external modules
$(call import-module, core/audio)
$(call import-module, thirdParty/png/android)
$(call import-module, thirdParty/z/android)
$(call import-module, thirdParty/OpenAL/android)
