# WkCore Library built as static library
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := wkcore_static
LOCAL_MODULE_FILENAME := libwkcore_static

LOCAL_CPP_EXTENSION := .cc

LOCAL_SRC_FILES :=    src/Time.cc       src/Randomizer.cc  src/Logger.cc 
LOCAL_SRC_FILES +=    src/NewLogger.cc  src/dbgmem.c 
LOCAL_SRC_FILES += src/Assert.cc

LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_C_INCLUDES := include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(BUILD_STATIC_LIBRARY)
