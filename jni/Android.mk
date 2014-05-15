LOCAL_PATH:=$(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE:=masonsdkjni

LOCAL_SRC_FILES:=myjni.cpp myjni.h

include $(BUILD_SHARED_LIBRARY)
