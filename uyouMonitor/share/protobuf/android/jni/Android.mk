LOCAL_PATH := $(call my-dir)
include $(CLEAR_VARS)

LOCAL_C_INCLUDES += $(LOCAL_PATH)/../..

LOCAL_CFLAGS := -fvisibility=hidden

LOCAL_CPP_EXTENSION := .cc  .cpp 

LOCAL_MODULE    := libprotobuf
	
PROTOBUF_SRC_LIST := $(wildcard $(LOCAL_PATH)/../../google/protobuf/*.cc)
LOCAL_SRC_FILES += $(PROTOBUF_SRC_LIST:$(LOCAL_PATH)/%=%)

PROTOBUF_COMPILER_SRC_LIST := $(wildcard $(LOCAL_PATH)/../../google/protobuf/compiler/*.cc)
LOCAL_SRC_FILES += $(PROTOBUF_COMPILER_SRC_LIST:$(LOCAL_PATH)/%=%)

PROTOBUF_IO_SRC_LIST := $(wildcard $(LOCAL_PATH)/../../google/protobuf/io/*.cc)
LOCAL_SRC_FILES += $(PROTOBUF_IO_SRC_LIST:$(LOCAL_PATH)/%=%)

PROTOBUF_STUBS_SRC_LIST := $(wildcard $(LOCAL_PATH)/../../google/protobuf/stubs/*.cc)
LOCAL_SRC_FILES += $(PROTOBUF_STUBS_SRC_LIST:$(LOCAL_PATH)/%=%)


#include $(BUILD_SHARED_LIBRARY)
include $(BUILD_STATIC_LIBRARY)

#下面把编译完成的文件copy到指定目录下
#$(info $(LOCAL_PATH)/../../../share/lib/android/$(TARGET_ARCH_ABI)/$(notdir $(LOCAL_BUILT_MODULE)))

all: $(LOCAL_PATH)/../../../share/lib/android/$(TARGET_ARCH_ABI)/$(notdir $(LOCAL_BUILT_MODULE))

$(LOCAL_PATH)/../../../share/lib/android/$(TARGET_ARCH_ABI)/$(notdir $(LOCAL_BUILT_MODULE)): $(LOCAL_BUILT_MODULE)
	CopyLibs.bat 