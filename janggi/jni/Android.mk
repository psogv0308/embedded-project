LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)
LOCAL_LDLIBS := -L$(SYSROOT)/usr/lib -llog
LOCAL_MODULE := janggi_c
LOCAL_SRC_FILES := janggi_c.c
include $(BUILD_SHARED_LIBRARY)
