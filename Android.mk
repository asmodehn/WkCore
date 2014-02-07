# WkCore Library built as static library
LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_MODULE := wkcore_static
LOCAL_MODULE_FILENAME := libwkcore_static

LOCAL_CPP_EXTENSION := .cc .cpp

LOCAL_CPP_FEATURES := rtti exceptions

LOCAL_SRC_FILES := \
src/Time.cc \
src/Randomizer.cpp \
src/Logger.cc \
src/NewLogger.cc \
src/dbgmem.c \
src/Assert.cc \
src/Logging/loglevel.cc \
src/Logging/logstream.cc \
src/Logging/logstreambuf.cc \
src/Logging/nullstream.cc \
src/Logging/nullstreambuf.cc 

#src/Error/ErrorCategory \
#src/Error/GenericError.cc \
#src/Error/GenericErrorCategory.cc \
#src/Error/SystemErrorCategory \

LOCAL_CFLAGS += -fvisibility=hidden

LOCAL_C_INCLUDES := $(LOCAL_PATH)/include

LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include

include $(BUILD_STATIC_LIBRARY)
