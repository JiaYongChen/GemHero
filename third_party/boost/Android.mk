LOCAL_PATH:= $(call my-dir)  
#LOCAL_CPPFLAGS += –fexceptions
include $(CLEAR_VARS)  
LOCAL_MODULE:= boost_system  
LOCAL_SRC_FILES:= android/lib/libboost_system.a  
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include  
include $(PREBUILT_STATIC_LIBRARY)  
   
include $(CLEAR_VARS)  
LOCAL_MODULE:= boost_serialization
LOCAL_SRC_FILES:= android/lib/libboost_serialization.a  
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include  
include $(PREBUILT_STATIC_LIBRARY)  
   
include $(CLEAR_VARS)  
LOCAL_MODULE:= boost_wserialization
LOCAL_SRC_FILES:= android/lib/libboost_wserialization.a  
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include  
include $(PREBUILT_STATIC_LIBRARY)

include $(CLEAR_VARS)  
LOCAL_MODULE:= boost_filesystem
LOCAL_SRC_FILES:= android/lib/libboost_filesystem.a  
LOCAL_EXPORT_C_INCLUDES := $(LOCAL_PATH)/include  
include $(PREBUILT_STATIC_LIBRARY)