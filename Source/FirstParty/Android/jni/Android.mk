LOCAL_PATH := $(call my-dir)

include $(CLEAR_VARS)

LOCAL_MODULE    := RicketyRacquet

LOCAL_C_INCLUDES := ../../Src

LOCAL_CPPFLAGS := -fexceptions -frtti -DANDROID=1
FILE_LIST := JNIHelper.cpp
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/animation/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/animation/provider/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/gui/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/gui/hud/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/model/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/model/collision/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/model/collision/filter/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/model/collision/handler/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/model/joint/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/rendering/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/rendering/parameter/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/rendering/transitions/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/resources/*.cpp)
FILE_LIST += $(wildcard $(LOCAL_PATH)/../../Src/state/*.cpp)
LOCAL_SRC_FILES := $(FILE_LIST:$(LOCAL_PATH)/%=%)

LOCAL_SHARED_LIBRARIES := sfml-system
LOCAL_SHARED_LIBRARIES += sfml-window
LOCAL_SHARED_LIBRARIES += sfml-graphics
LOCAL_SHARED_LIBRARIES += sfml-audio
LOCAL_SHARED_LIBRARIES += sfml-network
LOCAL_STATIC_LIBRARIES += libc++abi
LOCAL_WHOLE_STATIC_LIBRARIES := sfml-main
LOCAL_WHOLE_STATIC_LIBRARIES += tinyxml2
LOCAL_WHOLE_STATIC_LIBRARIES += Box2D
LOCAL_WHOLE_STATIC_LIBRARIES += CryptoPP

include $(BUILD_SHARED_LIBRARY)

$(call import-module,sfml)
$(call import-module,tinyxml2)
$(call import-module,Box2D)
$(call import-module,CryptoPP)
