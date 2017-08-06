#include <memory>

#include <android/log.h>
#include <jni.h>

#include "AppDelegate.h"

#define  LOG_TAG    "main"
#define  LOGD(...)  __android_log_print(ANDROID_LOG_DEBUG,LOG_TAG,__VA_ARGS__)

namespace {
std::unique_ptr<AppDelegate> appDelegate;
}

void cocos_android_app_init(JNIEnv* env) {
    LOGD("cocos_android_app_init");
    appDelegate.reset(new AppDelegate());
}

#ifdef ANDROID
int wctomb(char *s, wchar_t wc) { return wcrtomb(s,wc,NULL); }
int mbtowc(wchar_t *pwc, const char *s, size_t n) { return mbrtowc(pwc, s, n, NULL); }
#endif