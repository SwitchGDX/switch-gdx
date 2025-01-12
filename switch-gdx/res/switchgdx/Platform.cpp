#include "Clearwing.h"

#ifdef __SWITCH__
#include <switch.h>
#endif

const char *getOSLanguage() {
#ifdef __SWITCH__
    return "en-US";
//    u64 languageCode;
//    setGetSystemLanguage(&languageCode);
//    return (char *)&languageCode;
#else
    return "en-US";
#endif
}

const char *getSystemProperty(const char *key) {
    std::string name(key);
#if defined(__SWITCH__)
    if (name == "os.name")
        return "horizon";
    if (name == "os.arch")
        return "aarch64";
#elif defined(__WINRT__)
    if (name == "os.name")
        return "uwp";
    if (name == "os.arch")
        return "x86_64";
#else
    if (name == "os.name")
        return "unknown";
    if (name == "os.arch")
        return "x86_64";
#endif
    if (name == "line.separator")
        return "\n";
    if (name == "java.io.tmpdir")
        return "temp";
    if (name == "user.home")
        return "home";
    if (name == "user.name")
        return "user";
    return nullptr;
}
