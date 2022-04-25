#include "cn1_globals.h"

JAVA_OBJECT getOSLanguage(CODENAME_ONE_THREAD_STATE) {
#ifdef __SWITCH__
    u64 languageCode;
    setGetSystemLanguage(&languageCode);
    return fromNativeString(threadStateData, (char *)&languageCode);
#else
    return fromNativeString(threadStateData, "en-US");
#endif
}

JAVA_OBJECT getSystemProperty(CODENAME_ONE_THREAD_STATE, const char *key) {
#ifdef __SWITCH__
    if (!strcmp(key, "os.name"))
        return fromNativeString(threadStateData, "horizon");
    if (!strcmp(key, "os.arch"))
        return fromNativeString(threadStateData, "aarch64");
#else
    if (!strcmp(key, "os.name"))
        return fromNativeString(threadStateData, "unknown");
    if (!strcmp(key, "os.arch"))
        return fromNativeString(threadStateData, "x86_64");
#endif
    return JAVA_NULL;
}