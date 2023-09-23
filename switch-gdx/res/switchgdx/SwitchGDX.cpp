#include "Clearwing.h"

#include <java/lang/String.h>
#include <java/nio/ByteBuffer.h>
#include <java/nio/CharBuffer.h>
#include <java/nio/ShortBuffer.h>
#include <java/nio/FloatBuffer.h>
#include <java/nio/DoubleBuffer.h>
#include <java/nio/IntBuffer.h>
#include <java/nio/LongBuffer.h>
#include <java/io/IOException.h>

#include <com/thelogicmaster/switchgdx/SwitchApplication.h>
#include <com/thelogicmaster/switchgdx/SwitchAudio.h>
#include <com/thelogicmaster/switchgdx/SwitchMusic.h>
#include <com/thelogicmaster/switchgdx/SwitchGraphics.h>
#include <com/thelogicmaster/switchgdx/SwitchGL.h>
#include <com/thelogicmaster/switchgdx/SwitchInput.h>
#include <com/thelogicmaster/switchgdx/SwitchControllerManager.h>
#include <com/thelogicmaster/switchgdx/SwitchSound.h>
#include <com/thelogicmaster/switchgdx/SwitchNet.h>
#include <com/thelogicmaster/switchgdx/SwitchFiles.h>
#include <com/thelogicmaster/switchgdx/SwitchHttpResponse.h>
#include <com/thelogicmaster/switchgdx/SwitchSocket.h>
#include <com/thelogicmaster/switchgdx/SwitchServerSocket.h>
#include <com/badlogic/gdx/utils/GdxRuntimeException.h>
#include <com/badlogic/gdx/Input_TextInputListener.h>
#include <com/badlogic/gdx/utils/BufferUtils.h>
#include <com/badlogic/gdx/math/Matrix4.h>
#include <com/badlogic/gdx/graphics/g2d/Gdx2DPixmap.h>
#include <com/badlogic/gdx/graphics/glutils/ETC1.h>

#include <fcntl.h>
#include <csignal>
#include <vector>
#include <string>
#include "switchgdx/gdx_buffer_utils.h"
#include "switchgdx/gdx_matrix4.h"
#include "switchgdx/gdx2d.h"
#include "switchgdx/etc1_utils.h"
#include "switchgdx/tinyfiledialogs.h"
#include <curl/curl.h>
#include <chrono>
#include <SDL.h>
#include "switchgdx/SDL_mixer.h"
#include <SDL_gamecontroller.h>

#if !defined(__WIN32__) && !defined(__WINRT__)
# include <sys/socket.h>
# include <arpa/inet.h>
# include <netinet/in.h>
# include <netdb.h>
#else
# include <winsock2.h>
# include <ws2tcpip.h>
typedef int socklen_t;
#endif

#ifdef __WINRT__
#include "winrt/base.h"
namespace winrt::impl {
    template <typename Async>
    auto wait_for(Async const& async, Windows::Foundation::TimeSpan const& timeout);
}
#include <Windows.h>
#include <winrt/base.h>
#include <winrt/Windows.Storage.h>
std::string getLocalPathUWP() {
    auto path = winrt::Windows::Storage::ApplicationData::Current().LocalFolder().Path();
    return std::string(path.begin(), path.end());
}

#define main main
extern "C" extern int main(int argc, char* args[]);
int __stdcall wWinMain(HINSTANCE, HINSTANCE, PWSTR, int) {
    AllocConsole();
    FILE* fpstdin = stdin, * fpstdout = stdout, * fpstderr = stderr;
    freopen_s(&fpstdin, "CONIN$", "r", stdin);
    freopen_s(&fpstdout, "CONOUT$", "w", stdout);
    freopen_s(&fpstderr, "CONOUT$", "w", stderr);

    SDL_WinRTRunApp(main, nullptr);
}
#endif

#ifdef __SWITCH__
# include <switch.h>
# include <EGL/egl.h>
# include <EGL/eglext.h>
# include <glad/glad.h>
# include <stdio.h>
# include <sys/socket.h>
# include <arpa/inet.h>
# include <sys/errno.h>
# include <unistd.h>
#else
#include "glad/gles2.h"
#endif

static int touches[16 * 3];

#ifdef __SWITCH__
static EGLDisplay display;
static EGLContext context;
static EGLSurface surface;

static PadState combinedPad;
static PadState pads[8];

static int nxlinkSock = -1;
static bool socketInit;
#else
static SDL_Window *window;
static int buttons;
static float joysticks[4];
#endif

#ifdef __SWITCH__
extern "C" void userAppInit() {
    socketInitializeDefault();
    nxlinkStdio();
}

extern "C" void userAppExit() {
    socketExit();
}
#endif

void onMusicFinished() {
    // Todo: TLS context storage or store event in a queue until the next update loop
//    SM_com_thelogicmaster_switchgdx_SwitchAudio_onMusicFinished(ctx);
}

void onSoundFinished(int channel) {
    // Todo: TLS context storage or store event in a queue until the next update loop
//    SM_com_thelogicmaster_switchgdx_SwitchAudio_onSoundFinished_int(ctx, channel);
}

void SM_com_thelogicmaster_switchgdx_SwitchApplication_init_boolean(jcontext ctx, jbool vsync) {
    for (int i = 0; i < 16; i++)
        touches[i * 3] = -1;

#if defined(__WIN32__) || defined(__WINRT__)
    WSADATA wsa_data;
    if (WSAStartup(MAKEWORD(2, 2), &wsa_data))
        vm::throwNew<java::io::IOException>();
#endif

#ifdef __SWITCH__
    padConfigureInput(8, HidNpadStyleSet_NpadStandard);
    padInitializeAny(&combinedPad);

    padInitializeDefault(&pads[0]);
    for (int i = 1; i < 8; i++)
        padInitialize(&pads[i], static_cast<HidNpadIdType>(HidNpadIdType_No1 + i));

    setInitialize();

    hidInitializeTouchScreen();

    Result result = romfsInit();
    if (R_FAILED(result))
        Todo: Error handling/logging
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    eglInitialize(display, NULL, NULL);
    eglBindAPI(EGL_OPENGL_API);
    EGLConfig config;
    EGLint numConfigs;
    static const EGLint framebufferAttributeList[] = {
        EGL_RENDERABLE_TYPE, EGL_OPENGL_BIT,
        EGL_RED_SIZE,     8,
        EGL_GREEN_SIZE,   8,
        EGL_BLUE_SIZE,    8,
        EGL_ALPHA_SIZE,   8,
        EGL_DEPTH_SIZE,   24,
        EGL_STENCIL_SIZE, 8,
        EGL_NONE
    };
    eglChooseConfig(display, framebufferAttributeList, &config, 1, &numConfigs);
    surface = eglCreateWindowSurface(display, config, nwindowGetDefault(), NULL);
    static const EGLint contextAttributeList[] =
    {
        EGL_CONTEXT_OPENGL_PROFILE_MASK_KHR, EGL_CONTEXT_OPENGL_CORE_PROFILE_BIT_KHR,
        EGL_CONTEXT_MAJOR_VERSION_KHR, 2,
        EGL_CONTEXT_MINOR_VERSION_KHR, 0,
        EGL_NONE
    };
    context = eglCreateContext(display, config, EGL_NO_CONTEXT, contextAttributeList);
    eglMakeCurrent(display, surface, surface, context);
    gladLoadGL();

    SDL_Init(SDL_INIT_AUDIO);
#else
    SDL_Init(SDL_INIT_VIDEO | SDL_INIT_AUDIO | SDL_INIT_GAMECONTROLLER);

    window = SDL_CreateWindow("SwitchGDX", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 1280, 720, SDL_WINDOW_OPENGL | SDL_WINDOW_RESIZABLE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);

#ifdef __WINRT__
    SDL_SetHint(SDL_HINT_OPENGL_ES_DRIVER, "1");
    SDL_SetHint("SDL_WINRT_HANDLE_BACK_BUTTON", "1");
    SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 2);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_ES);
#endif

    auto context = SDL_GL_CreateContext(window);
    SDL_GL_MakeCurrent(window, context);
    SDL_GL_SetSwapInterval(vsync ? 1 : 0);

    gladLoadGLES2((GLADloadfunc) SDL_GL_GetProcAddress);
#endif

    Mix_Init(MIX_INIT_MP3 | MIX_INIT_OGG);
    Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, MIX_DEFAULT_CHANNELS, 4096);
    Mix_AllocateChannels(32);
    Mix_HookMusicFinished(onMusicFinished);
    Mix_ChannelFinished(onSoundFinished);

    curl_global_init(CURL_GLOBAL_ALL);
}

void SM_com_thelogicmaster_switchgdx_SwitchApplication_dispose(jcontext ctx) {
#if defined(__WIN32__) || defined(__WINRT__)
    WSACleanup();
#endif

#ifdef __SWITCH__
    if (display) {
        eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
        if (context)
            eglDestroyContext(display, context);
        if (surface)
            eglDestroySurface(display, surface);
        eglTerminate(display);
    }

    Mix_Quit();
    SDL_Quit();

    romfsExit();
#endif

    curl_global_cleanup();
}

#ifndef __SWITCH__
static int keyToButton(int key) {
    switch (key) {
        case SDL_SCANCODE_Z:
            return 1 << 3; // Y
        case SDL_SCANCODE_X:
            return 1 << 1; // B
        case SDL_SCANCODE_C:
            return 1 << 0; // A
        case SDL_SCANCODE_V:
            return 1 << 2; // X
        case SDL_SCANCODE_F:
            return 1 << 4; // Left stick
        case SDL_SCANCODE_G:
            return 1 << 5; // Right stick
        case SDL_SCANCODE_Q:
            return 1 << 6; // L
        case SDL_SCANCODE_E:
            return 1 << 7; // R
        case SDL_SCANCODE_R:
            return 1 << 8; // ZL
        case SDL_SCANCODE_T:
            return 1 << 9; // ZR
        case SDL_SCANCODE_N:
            return 1 << 10; // Plus
        case SDL_SCANCODE_M:
            return 1 << 11; // Minus
        case SDL_SCANCODE_UP:
            return 1 << 13; // D-up
        case SDL_SCANCODE_DOWN:
            return 1 << 15; // D-down
        case SDL_SCANCODE_LEFT:
            return 1 << 12; // D-left
        case SDL_SCANCODE_RIGHT:
            return 1 << 14; // D-right
        default:
            return 0;
    }
}

static int keyToAxis(int scancode) {
    switch (scancode) {
        case SDL_SCANCODE_W:
            return 0x4 + 1;
        case SDL_SCANCODE_S:
            return 1;
        case SDL_SCANCODE_A:
            return 0x4 + 0;
        case SDL_SCANCODE_D:
            return 0;
        case SDL_SCANCODE_I:
            return 0x4 + 3;
        case SDL_SCANCODE_K:
            return 3;
        case SDL_SCANCODE_J:
            return 0x4 + 2;
        case SDL_SCANCODE_L:
            return 2;
        default:
            return -1;
    }
}

static int mapButtonSDL(int button) {
    switch (button) {
        case SDL_CONTROLLER_BUTTON_A:
            return 1 << 0;
        case SDL_CONTROLLER_BUTTON_B:
            return 1 << 1;
        case SDL_CONTROLLER_BUTTON_X:
            return 1 << 2;
        case SDL_CONTROLLER_BUTTON_Y:
            return 1 << 3;
        case SDL_CONTROLLER_BUTTON_LEFTSTICK:
            return 1 << 4;
        case SDL_CONTROLLER_BUTTON_RIGHTSTICK:
            return 1 << 5;
        case SDL_CONTROLLER_BUTTON_LEFTSHOULDER:
            return 1 << 6;
        case SDL_CONTROLLER_BUTTON_RIGHTSHOULDER:
            return 1 << 7;
        case SDL_CONTROLLER_BUTTON_START:
            return 1 << 10;
        case SDL_CONTROLLER_BUTTON_BACK:
            return 1 << 11;
        case SDL_CONTROLLER_BUTTON_DPAD_LEFT:
            return 1 << 12;
        case SDL_CONTROLLER_BUTTON_DPAD_UP:
            return 1 << 13;
        case SDL_CONTROLLER_BUTTON_DPAD_RIGHT:
            return 1 << 14;
        case SDL_CONTROLLER_BUTTON_DPAD_DOWN:
            return 1 << 15;
        default:
            return 0;
    }
}
#else
static u64 remapPadButtons(u64 buttons, u32 style) {
    u64 mapped = buttons;

    if (style & HidNpadStyleTag_NpadJoyLeft) {
        mapped &= ~(
            HidNpadButton_Left | HidNpadButton_Right | HidNpadButton_Up | HidNpadButton_Down |
            HidNpadButton_StickLLeft | HidNpadButton_StickLRight | HidNpadButton_StickLUp | HidNpadButton_StickLDown |
            HidNpadButton_LeftSL | HidNpadButton_LeftSR
        );

        if (buttons & HidNpadButton_Left)
            mapped |= HidNpadButton_B;
        if (buttons & HidNpadButton_Down)
            mapped |= HidNpadButton_A;
        if (buttons & HidNpadButton_Up)
            mapped |= HidNpadButton_Y;
        if (buttons & HidNpadButton_Right)
            mapped |= HidNpadButton_X;

        if (buttons & HidNpadButton_StickLLeft)
            mapped |= HidNpadButton_StickLDown;
        if (buttons & HidNpadButton_StickLDown)
            mapped |= HidNpadButton_StickLRight;
        if (buttons & HidNpadButton_StickLRight)
            mapped |= HidNpadButton_StickLUp;
        if (buttons & HidNpadButton_StickLUp)
            mapped |= HidNpadButton_StickLLeft;

        if (buttons & HidNpadButton_LeftSL)
            mapped |= HidNpadButton_L;
        if (buttons & HidNpadButton_LeftSR)
            mapped |= HidNpadButton_R;
    } else if (style & HidNpadStyleTag_NpadJoyRight) {
        mapped &= ~(
            HidNpadButton_A | HidNpadButton_B | HidNpadButton_X | HidNpadButton_Y |
            HidNpadButton_StickLLeft | HidNpadButton_StickLRight | HidNpadButton_StickLUp | HidNpadButton_StickLDown |
            HidNpadButton_LeftSL | HidNpadButton_LeftSR
        );

        if (buttons & HidNpadButton_A)
            mapped |= HidNpadButton_B;
        if (buttons & HidNpadButton_X)
            mapped |= HidNpadButton_A;
        if (buttons & HidNpadButton_B)
            mapped |= HidNpadButton_Y;
        if (buttons & HidNpadButton_Y)
            mapped |= HidNpadButton_X;

        if (buttons & HidNpadButton_StickRLeft)
            mapped |= HidNpadButton_StickRUp;
        if (buttons & HidNpadButton_StickRDown)
            mapped |= HidNpadButton_StickRLeft;
        if (buttons & HidNpadButton_StickRRight)
            mapped |= HidNpadButton_StickRDown;
        if (buttons & HidNpadButton_StickRUp)
            mapped |= HidNpadButton_StickRRight;

        if (buttons & HidNpadButton_RightSL)
            mapped |= HidNpadButton_L;
        if (buttons & HidNpadButton_RightSR)
            mapped |= HidNpadButton_R;
    }

    return mapped;
}

static void remapPadAxes(float *axes, u32 style) {
    if (style & HidNpadStyleTag_NpadJoyLeft) {
        float temp = axes[0];
        axes[0] = -axes[1];
        axes[1] = temp;
    } else if(style & HidNpadStyleTag_NpadJoyRight) {
        axes[0] = axes[3];
        axes[1] = -axes[2];
        axes[2] = 0;
        axes[3] = 0;
    }
}
#endif

jbool SM_com_thelogicmaster_switchgdx_SwitchApplication_update_R_boolean(jcontext ctx) {
#ifdef __SWITCH__
    padUpdate(&combinedPad);
    u64 kDown = padGetButtonsDown(&combinedPad);
    if (kDown & HidNpadButton_Plus)
        return false;

    for (int i = 0; i < 8; i++)
        padUpdate(&pads[i]);

    HidTouchScreenState touchState;
    if (hidGetTouchScreenStates(&touchState, 1)) {
        for (int i = 0; i < 16; i++)
            if (i < touchState.count) {
                touches[i * 3 + 0] = touchState.touches[i].finger_id;
                touches[i * 3 + 1] = touchState.touches[i].x;
                touches[i * 3 + 2] = touchState.touches[i].y;
            } else {
                touches[i * 3 + 0] = -1;
                touches[i * 3 + 1] = 0;
                touches[i * 3 + 2] = 0;
            }
    }

    eglSwapBuffers(display, surface);
    return appletMainLoop();
#else
    SDL_Event event;
    int axis;
    while (SDL_PollEvent(&event))
        switch (event.type) {
            case SDL_QUIT:
                return false;
            case SDL_MOUSEMOTION:
                touches[1] = event.motion.x;
                touches[2] = event.motion.y;
                break;
            case SDL_MOUSEBUTTONDOWN:
                touches[0] = 0;
                touches[1] = event.button.x;
                touches[2] = event.button.y;
                break;
            case SDL_MOUSEBUTTONUP:
                touches[0] = -1;
                break;
            case SDL_KEYDOWN:
                buttons |= keyToButton(event.key.keysym.scancode);
                axis = keyToAxis(event.key.keysym.scancode);
                if (axis > -1 and !event.key.repeat)
                    joysticks[axis & 0x3] += axis & 0x4 ? -1 : 1;
                break;
            case SDL_KEYUP:
                buttons &= ~keyToButton(event.key.keysym.scancode);
                axis = keyToAxis(event.key.keysym.scancode);
                if (axis > -1 and !event.key.repeat)
                    joysticks[axis & 0x3] = 0;
                break;
            case SDL_CONTROLLERBUTTONDOWN:
                buttons |= mapButtonSDL(event.cbutton.button);
                break;
            case SDL_CONTROLLERBUTTONUP:
                buttons &= ~mapButtonSDL(event.cbutton.button);
                break;
            case SDL_CONTROLLERAXISMOTION:
                if (event.caxis.axis >= 0 && event.caxis.axis < 4)
                    joysticks[event.caxis.axis] = (float)event.caxis.value / 32768.f;
                for (int i = 0; i < 2; i++)
                    if (event.caxis.axis == SDL_CONTROLLER_AXIS_TRIGGERLEFT + i) {
                        if (event.caxis.value > 512)
                            buttons |= 1 << (8 + i);
                        else
                            buttons &= ~(1 << (8 + i));
                    }
                break;
            case SDL_CONTROLLERDEVICEADDED:
                SDL_GameControllerOpen(event.cdevice.which);
                break;
            case SDL_CONTROLLERDEVICEREMOVED:
                SDL_GameControllerClose(SDL_GameControllerFromPlayerIndex(event.cdevice.which));
                break;
        }

    SDL_GL_SwapWindow(window);
    return true;
#endif
}

jobject M_com_thelogicmaster_switchgdx_SwitchFiles_getLocalStoragePath_R_java_lang_String(jcontext ctx, jobject self) {
#ifdef __WINRT__
    auto path = getLocalPathUWP();
    return (jobject) stringFromNative(ctx, path.c_str());
#else
    return (jobject) stringFromNative(ctx, "data");
#endif
}

jbool M_com_thelogicmaster_switchgdx_SwitchNet_openURI_java_lang_String_R_boolean(jcontext ctx, jobject self, jobject urlObj) {
#ifdef __SWITCH__
    WebCommonConfig config;
    WebCommonReply reply;
    return !webPageCreate(&config, vm::getNativeString(urlObj)) and !webConfigSetWhitelist(&config, "^http*") and !webConfigShow(&config, &reply);
#else
    std::string url(stringToNative(ctx, (jstring) urlObj));
# if defined(__WIN32__) || defined(__WINRT__)
    system(("start " + url).c_str());
# elif __APPLE__
    system(("open " + url).c_str());
# else
    system(("xdg-open " + url).c_str());
# endif
    return true;
#endif
}

static size_t curlWriteCallback(void *contents, size_t size, size_t nmemb, void *string) {
    auto *data = (std::string *)string;
    data->append((const char *)contents, size * nmemb);
    return size * nmemb;
}

jobject SM_com_thelogicmaster_switchgdx_SwitchNet_sendRequest_java_lang_String_Array1_byte_Array1_java_lang_String_java_lang_String_long_com_thelogicmaster_switchgdx_SwitchHttpResponse_R_java_lang_String
        (jcontext ctx, jobject urlObj, jobject contentArray, jobject headersArray, jobject methodObj, jlong timeout, jobject httpResponse) {
    char errorBuffer[CURL_ERROR_SIZE]{};
    CURL *curl = curl_easy_init();
    if (!curl)
        return nullptr;

    std::string responseData;
    curl_easy_setopt(curl, CURLOPT_URL, stringToNative(ctx, (jstring) urlObj));
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, curlWriteCallback);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, &responseData);
    curl_easy_setopt(curl, CURLOPT_USERAGENT, "switchgdx-agent/1.0");
    curl_easy_setopt(curl, CURLOPT_ERRORBUFFER, errorBuffer);

    curl_slist *headers = nullptr;
    for (int i = 0; i < ((jstring) headersArray)->F_count; i++)
        headers = curl_slist_append(headers, stringToNative(ctx, ((jstring *)((jarray) headersArray)->data)[i]));
    curl_easy_setopt(curl, CURLOPT_HTTPHEADER, headers);

    auto method = std::string(stringToNative(ctx, (jstring) methodObj));
    if (method == "POST" or method == "PUT" or method == "PATCH") {
        curl_easy_setopt(curl, CURLOPT_POSTFIELDSIZE, ((jarray) contentArray)->length);
        curl_easy_setopt(curl, CURLOPT_POSTFIELDS, ((jarray) contentArray)->data);
    } else if (method == "HEAD")
        curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
    else if (method == "GET")
        curl_easy_setopt(curl, CURLOPT_HTTPGET, 1L);

    curl_easy_setopt(curl, CURLOPT_TIMEOUT_MS, timeout);

    long status;
    CURLcode res = curl_easy_perform(curl);
    curl_easy_getinfo(curl, CURLINFO_RESPONSE_CODE, &status);

    curl_slist_free_all(headers);
    curl_easy_cleanup(curl);

    if (res != CURLE_OK)
        return (jobject) stringFromNative(ctx, strlen(errorBuffer) ? errorBuffer : curl_easy_strerror(res));

    ((com_thelogicmaster_switchgdx_SwitchHttpResponse *) httpResponse)->F_status = (jint)status;

    auto response = createArray(ctx, &class_byte, (int)responseData.length());
    memcpy(response->data, responseData.c_str(), responseData.length());
    ((com_thelogicmaster_switchgdx_SwitchHttpResponse *) httpResponse)->F_result = (jref) response;

    return nullptr;
}

void throwNativeSocketException(jcontext ctx, bool inErrno = false) {
    std::string error;
    if (errno == ETIMEDOUT)
        error = "Timed out";
    else {
#if defined(__WIN32__)
        if (!inErrno)
            errno = WSAGetLastError();
        char buffer[256];
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS | FORMAT_MESSAGE_MAX_WIDTH_MASK, nullptr, errno, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), buffer, sizeof(buffer), nullptr);
        error = buffer;
#elif defined(__WINRT__)
        error = "Socket error";
#else
        error = strerror(errno);
#endif
    }
    throwIOException(ctx, error.c_str());
}

void M_com_thelogicmaster_switchgdx_SwitchSocket_dispose(jcontext ctx, jobject self) {
    auto &fd = ((com_thelogicmaster_switchgdx_SwitchSocket *) self)->F_fd;
    if (fd) {
#if defined(__WIN32__) || defined(__WINRT__)
        shutdown(fd, SD_SEND);
        closesocket(fd);
#else
        close(fd);
#endif
        fd = 0;
    }
}

void setSocketTimeout(int fd, int timeout) {
#if defined(__WIN32__) || defined(__WINRT__)
    DWORD timeoutVal = timeout;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, (const char*)&timeoutVal, sizeof timeoutVal);
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, (const char*)&timeoutVal, sizeof timeoutVal);
#else
    timeout /= 2; // Timeout doesn't seem to be reliable in blocking mode
    timeval timeoutVal{};
    timeoutVal.tv_usec = 1000 * (timeout % 1000);
    timeoutVal.tv_sec = timeout / 1000;
    setsockopt(fd, SOL_SOCKET, SO_RCVTIMEO, &timeoutVal, sizeof timeoutVal);
    setsockopt(fd, SOL_SOCKET, SO_SNDTIMEO, &timeoutVal, sizeof timeoutVal);
#endif
}

jint SM_com_thelogicmaster_switchgdx_SwitchSocket_create_java_lang_String_int_int_int_R_int(jcontext ctx, jobject hostObj, jint port, jint connectTimeout, jint timeout) {
//    std::string host(hostObj ? vm::getNativeString(hostObj) : "");
//    const char *hostname = nullptr;
//    if (hostObj)
//        hostname = host.c_str();
//
//    int fd = -1;
//
//    addrinfo hints{};
//    addrinfo *addrInfo = nullptr, *addrInfoIter = nullptr;
//    in6_addr serverAddr{};
//    hints.ai_flags = AI_NUMERICSERV;
//    hints.ai_family = AF_UNSPEC;
//    hints.ai_socktype = SOCK_STREAM;
//
//    auto portString = std::to_string(port);
//
//    if (hostname) {
//        if (inet_pton(AF_INET, hostname, &serverAddr) == 1) {
//            hints.ai_family = AF_INET;
//            hints.ai_flags |= AI_NUMERICHOST;
//        } else if (inet_pton(AF_INET6, hostname, &serverAddr) == 1) {
//            hints.ai_family = AF_INET6;
//            hints.ai_flags |= AI_NUMERICHOST;
//        }
//    }
//
//    if (getaddrinfo(hostname, portString.c_str(), &hints, &addrInfo))
//        goto error;
//
//    for (addrInfoIter = addrInfo; addrInfoIter; addrInfoIter = addrInfoIter->ai_next) {
//        fd = socket(addrInfoIter->ai_family, addrInfoIter->ai_socktype, addrInfoIter->ai_protocol);
//        if (fd < 0)
//            goto error;
//
//#if defined(__WIN32__) || defined(__WINRT__)
//        u_long nonblocking = 1;
//        ioctlsocket(fd, FIONBIO, &nonblocking);
//#else
//        if (fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK) < 0)
//            goto error;
//#endif
//
//        if (connect(fd, addrInfoIter->ai_addr, (int)addrInfoIter->ai_addrlen)) {
//#if defined(__WIN32__) || defined(__WINRT__)
//            if (WSAGetLastError() == WSAEWOULDBLOCK) {
//#else
//            if (errno == EWOULDBLOCK or errno == EINPROGRESS) {
//#endif
//                auto start = std::chrono::steady_clock::now();
//                int remaining;
//                while (true) {
//                    remaining = connectTimeout - std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count();
//                    if (remaining <= 0) {
//                        fd = -1;
//                        errno = ETIMEDOUT;
//                        break;
//                    }
//                    fd_set wr{}, ex{};
//                    timeval t{};
//                    FD_SET(fd, &wr);
//                    FD_SET(fd, &ex);
//                    t.tv_sec = remaining / 1000;
//                    t.tv_usec = (remaining % 1000) * 1000;
//                    int rc = select(fd + 1, nullptr, &wr, &ex, &t);
//                    if (rc < 0 && errno == EINTR)
//                        continue;
//                    if (rc == 0 or FD_ISSET(fd, &ex))
//                        fd = -1;
//                    if (rc == 0)
//                        errno = ETIMEDOUT;
//                    break;
//                }
//                if (fd >= 0)
//                    break;
//            } else {
//#if defined(__WIN32__) || defined(__WINRT__)
//                shutdown(fd, SD_SEND);
//                closesocket(fd);
//#else
//                close(fd);
//#endif
//                fd = -1;
//            }
//        } else
//            break;
//    }
//
//    if (fd < 0)
//        goto error;
//
//#if defined(__WIN32__) || defined(__WINRT__)
//    {
//        u_long nonblocking = 0;
//        ioctlsocket(fd, FIONBIO, &nonblocking);
//    }
//#else
//    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & ~O_NONBLOCK);
//#endif
//
//    setSocketTimeout(fd, timeout);
//
//    freeaddrinfo(addrInfo);
//    return fd;
//
//    error:
//#if defined(__WIN32__) || defined(__WINRT__)
//    errno = WSAGetLastError();
//#endif
//
//    if (fd > 0) {
//#if defined(__WIN32__) || defined(__WINRT__)
//        shutdown(fd, SD_SEND);
//        closesocket(fd);
//#else
//        close(fd);
//#endif
//    }
//    if (addrInfo)
//        freeaddrinfo(addrInfo);
//    throwNativeSocketException(true);
    return 0;
}

jint M_com_thelogicmaster_switchgdx_SwitchSocket_read_R_int(jcontext ctx, jobject self) {
//    signed char buffer;
//    if (!F_fd)
//        vm::throwNew<java::io::IOException>();
//    if (recv(F_fd, (char *)&buffer, 1, 0) != 1) {
//#if defined(__WIN32__) || defined(__WINRT__)
//        errno = WSAGetLastError();
//#endif
//        throwNativeSocketException(true);
//    }
//    return buffer;
    return 0;
}

void M_com_thelogicmaster_switchgdx_SwitchSocket_write_int(jcontext ctx, jobject self, jint value) {
//    auto buffer = (signed char) value;
//    if (!F_fd)
//        vm::throwNew<java::io::IOException>();
//    if (send(F_fd, (char *)&buffer, 1, 0) != 1) {
//#if defined(__WIN32__) || defined(__WINRT__)
//        errno = WSAGetLastError();
//#endif
//        throwNativeSocketException();
//    }
}

jobject M_com_thelogicmaster_switchgdx_SwitchSocket_getRemoteAddress_R_java_lang_String(jcontext ctx, jobject self) {
//    if (!F_fd)
//        vm::throwNew<java::io::IOException>();
//    sockaddr_storage address{};
//    socklen_t addrLen = sizeof(address);
//    char addrStr[INET6_ADDRSTRLEN];
//    if (getpeername(F_fd, (sockaddr *) &address, &addrLen))
//        throwNativeSocketException();
//    auto data = address.ss_family == AF_INET ? (void *)&((sockaddr_in *)&address)->sin_addr : (void *)&((sockaddr_in6 *)&address)->sin6_addr;
//    if (!inet_ntop(address.ss_family, data, addrStr, sizeof(addrStr)))
//        throwNativeSocketException();
//    return vm::createString(addrStr);
    return nullptr;
}

void M_com_thelogicmaster_switchgdx_SwitchServerSocket_dispose(jcontext ctx, jobject self) {
//    if (F_fd) {
//#if defined(__WIN32__) || defined(__WINRT__)
//        shutdown(F_fd, SD_SEND);
//        closesocket(F_fd);
//#else
//        close(F_fd);
//#endif
//        F_fd = 0;
//    }
}

jint SM_com_thelogicmaster_switchgdx_SwitchServerSocket_create_int_boolean_R_int(jcontext ctx,jint port, jbool reuseAddress) {
//    sockaddr_in address{};
//    address.sin_family = AF_INET;
//    address.sin_port = htons(port);
//    address.sin_addr.s_addr = INADDR_ANY;
//    int fd;
//
//    if ((fd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
//        goto error;
//
//    if (setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, (char *) &reuseAddress, sizeof(int)) < 0)
//        goto error;
//
//#if defined(__WIN32__) || defined(__WINRT__)
//    {
//        u_long nonblocking = 1;
//        ioctlsocket(fd, FIONBIO, &nonblocking);
//    }
//#else
//    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) | O_NONBLOCK);
//#endif
//
//    if (bind(fd, (sockaddr *) &address, sizeof(address)) < 0)
//        goto error;
//
//    if (listen(fd, 10) < 0)
//        goto error;
//
//    return fd;
//
//    error:
//#if defined(__WIN32__) || defined(__WINRT__)
//    errno = WSAGetLastError();
//#endif
//
//    if (fd > 0) {
//#if defined(__WIN32__) || defined(__WINRT__)
//        shutdown(fd, SD_SEND);
//        closesocket(fd);
//#else
//        close(fd);
//#endif
//    }
//    throwNativeSocketException(true);
    return 0;
}

jint M_com_thelogicmaster_switchgdx_SwitchServerSocket_accept_int_R_int(jcontext ctx, jobject self, jint timeout) {
//    if (!F_fd)
//        vm::throwNew<java::io::IOException>();
//
//    sockaddr_in6 address{};
//    socklen_t addrLen = sizeof(address);
//    if (getsockname(F_fd, (sockaddr *) &address, &addrLen))
//        throwNativeSocketException();
//
//    int fd = -1;
//    auto start = std::chrono::steady_clock::now();
//    while (std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::steady_clock::now() - start).count() < F_acceptTimeout) {
//        fd = accept(F_fd, (sockaddr *) &address, &addrLen);
//        if (fd >= 0)
//            break;
//#if defined(__WIN32__) || defined(__WINRT__)
//        if (WSAGetLastError() != WSAEWOULDBLOCK)
//#else
//        if (errno != EAGAIN)
//#endif
//            throwNativeSocketException();
//    }
//
//    if (fd < 0) {
//        auto error = java::io::IOException::newObject();
//        vm::checkedCast<java::io::IOException>(error)->init_java_lang_String(vm::createString("Accept timed out"));
//        throw error;
//    }
//
//#if defined(__WIN32__) || defined(__WINRT__)
//    {
//        u_long nonblocking = 0;
//        ioctlsocket(fd, FIONBIO, &nonblocking);
//    }
//#else
//    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL, 0) & ~O_NONBLOCK);
//#endif
//    setSocketTimeout(fd, timeout);
//    return fd;
    return 0;
}

void M_com_thelogicmaster_switchgdx_SwitchMusic_create_java_lang_String(jcontext ctx, jobject self, jobject file) {
    auto music = Mix_LoadMUS(stringToNative(ctx, (jstring) file));
    if (!music)
        constructAndThrowMsg<&class_com_badlogic_gdx_utils_GdxRuntimeException, init_java_lang_RuntimeException_java_lang_String>(ctx, Mix_GetError());
    ((com_thelogicmaster_switchgdx_SwitchMusic *) self)->F_handle = (jlong)music;
}

void M_com_thelogicmaster_switchgdx_SwitchMusic_start_boolean(jcontext ctx, jobject self, jbool looping) {
    Mix_PlayMusic((Mix_Music*)((com_thelogicmaster_switchgdx_SwitchMusic *) self)->F_handle, looping ? -1 : 0);
}

void SM_com_thelogicmaster_switchgdx_SwitchMusic_resume(jcontext ctx) {
    Mix_ResumeMusic();
}

void SM_com_thelogicmaster_switchgdx_SwitchMusic_pause0(jcontext ctx) {
    Mix_PauseMusic();
}

void SM_com_thelogicmaster_switchgdx_SwitchMusic_stop0(jcontext ctx) {
    Mix_HaltMusic();
}

void SM_com_thelogicmaster_switchgdx_SwitchMusic_setVolume0_float(jcontext ctx, jfloat volume) {
    Mix_VolumeMusic((int)(volume * MIX_MAX_VOLUME));
}

void SM_com_thelogicmaster_switchgdx_SwitchMusic_setPosition0_float(jcontext ctx, jfloat position) {
    Mix_RewindMusic();
    Mix_SetMusicPosition(position);
}

void M_com_thelogicmaster_switchgdx_SwitchMusic_dispose0(jcontext ctx, jobject self) {
    auto& handle = ((com_thelogicmaster_switchgdx_SwitchMusic *) self)->F_handle;
    if (!handle)
        return;
    Mix_FreeMusic((Mix_Music *)handle);
    handle = 0;
}

void M_com_thelogicmaster_switchgdx_SwitchSound_create_java_lang_String(jcontext ctx, jobject self, jobject file) {
    auto sound = Mix_LoadWAV(stringToNative(ctx, (jstring) file));
    if (!sound)
        constructAndThrowMsg<&class_com_badlogic_gdx_utils_GdxRuntimeException, init_java_lang_RuntimeException_java_lang_String>(ctx, Mix_GetError());
    ((com_thelogicmaster_switchgdx_SwitchSound *) self)->F_handle = (jlong)sound;
}

void M_com_thelogicmaster_switchgdx_SwitchSound_dispose0(jcontext ctx, jobject self) {
    auto &handle = ((com_thelogicmaster_switchgdx_SwitchSound *) self)->F_handle;
    if (!handle)
        return;
    Mix_FreeChunk((Mix_Chunk *)handle);
    handle = 0;
}

jint M_com_thelogicmaster_switchgdx_SwitchSound_play0_boolean_R_int(jcontext ctx, jobject self, jbool looping) {
    return Mix_PlayChannel(-1, (Mix_Chunk*)((com_thelogicmaster_switchgdx_SwitchSound *) self)->F_handle, looping ? -1 : 0);
}

void M_com_thelogicmaster_switchgdx_SwitchSound_setLooping0_int_boolean(jcontext ctx, jobject self, jint channel, jbool looping) {
    Mix_PlayChannel(channel, (Mix_Chunk*)((com_thelogicmaster_switchgdx_SwitchSound *) self)->F_handle, looping ? -1 : 0);
}

void SM_com_thelogicmaster_switchgdx_SwitchSound_stop0_int(jcontext ctx, jint channel) {
    Mix_HaltChannel(channel);
}

void SM_com_thelogicmaster_switchgdx_SwitchSound_pause0_int(jcontext ctx, jint channel) {
    Mix_Pause(channel);
}

void SM_com_thelogicmaster_switchgdx_SwitchSound_resume0_int(jcontext ctx, jint channel) {
    Mix_Resume(channel);
}

void SM_com_thelogicmaster_switchgdx_SwitchSound_setPitch0_int_float(jcontext ctx, jint channel, jfloat pitch) {
    // Todo: Custom pitch changing effect based on: https://gist.github.com/hydren/ea794e65e95c7713c00c88f74b71f8b1
}

void SM_com_thelogicmaster_switchgdx_SwitchSound_setVolume0_int_float(jcontext ctx, jint channel, jfloat volume) {
    Mix_Volume(channel, (int)(volume * MIX_MAX_VOLUME));
}

void SM_com_thelogicmaster_switchgdx_SwitchSound_setPan0_int_float(jcontext ctx, jint channel, jfloat pan) {
    uint8_t left, right;
    if (pan <= 0) {
        left = 255;
        right = (uint8_t)((1 + pan) * 255);
    } else {
        left = (uint8_t)((1 - pan) * 255);
        right = 255;
    }
    Mix_SetPanning(channel, left, right);
}

void *getBufferAddress(jcontext ctx, java_nio_Buffer *buffer) {
    if (!buffer)
        return nullptr;
    int typeSize = 1;
    if (isInstance(ctx, (jobject) buffer, &class_java_nio_ByteBuffer))
        typeSize = sizeof(jbyte);
    else if (isInstance(ctx, (jobject) buffer, &class_java_nio_FloatBuffer))
        typeSize = sizeof(jfloat);
    else if (isInstance(ctx, (jobject) buffer, &class_java_nio_IntBuffer))
        typeSize = sizeof(jint);
    else if (isInstance(ctx, (jobject) buffer, &class_java_nio_ShortBuffer))
        typeSize = sizeof(jshort);
    else if (isInstance(ctx, (jobject) buffer, &class_java_nio_CharBuffer))
        typeSize = sizeof(jchar);
    else if (isInstance(ctx, (jobject) buffer, &class_java_nio_LongBuffer))
        typeSize = sizeof(jlong);
    else if (isInstance(ctx, (jobject) buffer, &class_java_nio_DoubleBuffer))
        typeSize = sizeof(jdouble);
    return (char*)buffer->F_address + typeSize * buffer->F_position;
}

void *getBufferAddress(java_nio_ByteBuffer *buffer) {
    return (char*)buffer->parent.F_address + sizeof(jbyte) * buffer->parent.F_position;
}

void *getBufferAddress(java_nio_ShortBuffer *buffer) {
    return (char*)buffer->parent.F_address + sizeof(jshort) * buffer->parent.F_position;
}

void *getBufferAddress(java_nio_CharBuffer *buffer) {
    return (char*)buffer->parent.F_address + sizeof(jchar) * buffer->parent.F_position;
}

void *getBufferAddress(java_nio_IntBuffer *buffer) {
    return (char*)buffer->parent.F_address + sizeof(jint) * buffer->parent.F_position;
}

void *getBufferAddress(java_nio_LongBuffer *buffer) {
    return (char*)buffer->parent.F_address + sizeof(jlong) * buffer->parent.F_position;
}

void *getBufferAddress(java_nio_FloatBuffer *buffer) {
    return (char*)buffer->parent.F_address + sizeof(jfloat) * buffer->parent.F_position;
}

void *getBufferAddress(java_nio_DoubleBuffer *buffer) {
    return (char*)buffer->parent.F_address + sizeof(jdouble) * buffer->parent.F_position;
}

void SM_com_badlogic_gdx_utils_BufferUtils_freeMemory_java_nio_ByteBuffer(jcontext ctx, jobject buffer) {
    auto memory = (char *) ((java_nio_Buffer *) buffer)->F_address;
    if (memory) {
        delete[] memory;
        ((java_nio_Buffer *) buffer)->F_address = 0;
    }
}

jobject SM_com_badlogic_gdx_utils_BufferUtils_newDisposableByteBuffer_int_R_java_nio_ByteBuffer(jcontext ctx, jint size) {
    return SM_java_nio_ByteBuffer_allocateDirect_int_R_java_nio_ByteBuffer(ctx, size);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_getBufferAddress_java_nio_Buffer_R_long(jcontext ctx, jobject buffer) {
    return ((java_nio_Buffer *) buffer)->F_address;
}

void SM_com_badlogic_gdx_utils_BufferUtils_clear_java_nio_ByteBuffer_int(jcontext ctx, jobject buffer, jint numBytes) {
    memset((void *) ((java_nio_Buffer *) buffer)->F_address, 0, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_Array1_float_java_nio_Buffer_int_int(jcontext ctx, jobject src, jobject dst, jint numFloats, jint offset) {
    memcpy((float *) ((java_nio_Buffer *) dst)->F_address, (float *) ((jarray) src)->data + offset, numFloats << 2);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_Array1_byte_int_java_nio_Buffer_int_int(jcontext ctx, jobject src, jint srcOffset, jobject dst, jint dstOffset, jint numBytes) {
    memcpy((char *) ((java_nio_Buffer *) dst)->F_address + dstOffset, (jbyte *) ((jarray) src)->data + srcOffset, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_Array1_char_int_java_nio_Buffer_int_int(jcontext ctx, jobject src, jint srcOffset, jobject dst, jint dstOffset, jint numBytes) {
    memcpy((char *) ((java_nio_Buffer *) dst)->F_address + dstOffset, (jchar *) ((jarray) src)->data + srcOffset, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_Array1_short_int_java_nio_Buffer_int_int(jcontext ctx, jobject src, jint srcOffset, jobject dst, jint dstOffset, jint numBytes) {
    memcpy((char *) ((java_nio_Buffer *) dst)->F_address + dstOffset, (jshort *) ((jarray) src)->data + srcOffset, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_Array1_int_int_java_nio_Buffer_int_int(jcontext ctx, jobject src, jint srcOffset, jobject dst, jint dstOffset, jint numBytes) {
    memcpy((char *) ((java_nio_Buffer *) dst)->F_address + dstOffset, (jint *) ((jarray) src)->data + srcOffset, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_Array1_long_int_java_nio_Buffer_int_int(jcontext ctx, jobject src, jint srcOffset, jobject dst, jint dstOffset, jint numBytes) {
    memcpy((char *) ((java_nio_Buffer *) dst)->F_address + dstOffset, (jlong *) ((jarray) src)->data + srcOffset, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_Array1_float_int_java_nio_Buffer_int_int(jcontext ctx, jobject src, jint srcOffset, jobject dst, jint dstOffset, jint numBytes) {
    memcpy((char *) ((java_nio_Buffer *) dst)->F_address + dstOffset, (jfloat *) ((jarray) src)->data + srcOffset, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_Array1_double_int_java_nio_Buffer_int_int(jcontext ctx, jobject src, jint srcOffset, jobject dst, jint dstOffset, jint numBytes) {
    memcpy((char *) ((java_nio_Buffer *) dst)->F_address + dstOffset, (jdouble *) ((jarray) src)->data + srcOffset, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_copyJni_java_nio_Buffer_int_java_nio_Buffer_int_int(jcontext ctx, jobject src, jint srcOffset, jobject dst, jint dstOffset, jint numBytes) {
    memcpy((char *) ((java_nio_Buffer *) dst)->F_address + dstOffset, (char *) ((java_nio_Buffer *) src)->F_address + srcOffset, numBytes);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV4M4Jni_java_nio_Buffer_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<4, 4>((float *) ((java_nio_Buffer *) data)->F_address, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV4M4Jni_Array1_float_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<4, 4>((float *) ((jarray) data)->data, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV3M4Jni_java_nio_Buffer_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<3, 4>((float *) ((java_nio_Buffer *) data)->F_address, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV3M4Jni_Array1_float_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<3, 4>((float *) ((jarray) data)->data, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV2M4Jni_java_nio_Buffer_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<2, 4>((float *) ((java_nio_Buffer *) data)->F_address, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV2M4Jni_Array1_float_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<2, 4>((float *) ((jarray) data)->data, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV3M3Jni_java_nio_Buffer_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<3, 3>((float *) ((java_nio_Buffer *) data)->F_address, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV3M3Jni_Array1_float_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<3, 3>((float *) ((jarray) data)->data, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV2M3Jni_java_nio_Buffer_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<2, 3>((float *) ((java_nio_Buffer *) data)->F_address, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

void SM_com_badlogic_gdx_utils_BufferUtils_transformV2M3Jni_Array1_float_int_int_Array1_float_int(jcontext ctx, jobject data, jint strideInBytes, jint count, jobject matrix, jint offsetInBytes) {
    transform<2, 3>((float *) ((jarray) data)->data, strideInBytes / 4, count, (float *) ((jarray) matrix)->data, offsetInBytes / 4);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_find_java_nio_Buffer_int_int_java_nio_Buffer_int_int_R_long(jcontext ctx, jobject vertex, jint vertexOffsetInBytes, jint strideInBytes, jobject vertices, jint verticesOffsetInBytes, jint numVertices) {
    return find(&((float *) ((java_nio_Buffer *) vertex)->F_address)[vertexOffsetInBytes / 4], strideInBytes / 4, &((float *) ((java_nio_Buffer *) vertices)->F_address)[verticesOffsetInBytes / 4], numVertices);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_find_Array1_float_int_int_java_nio_Buffer_int_int_R_long(jcontext ctx, jobject vertex, jint vertexOffsetInBytes, jint strideInBytes, jobject vertices, jint verticesOffsetInBytes, jint numVertices) {
    return find(&((float *) ((jarray) vertex)->data)[vertexOffsetInBytes / 4], strideInBytes / 4, &((float *) ((java_nio_Buffer *) vertices)->F_address)[verticesOffsetInBytes / 4], numVertices);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_find_java_nio_Buffer_int_int_Array1_float_int_int_R_long(jcontext ctx, jobject vertex, jint vertexOffsetInBytes, jint strideInBytes, jobject vertices, jint verticesOffsetInBytes, jint numVertices) {
    return find(&((float *) ((java_nio_Buffer *) vertex)->F_address)[vertexOffsetInBytes / 4], strideInBytes / 4, &((float *) ((jarray) vertices)->data)[verticesOffsetInBytes / 4], numVertices);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_find_Array1_float_int_int_Array1_float_int_int_R_long(jcontext ctx, jobject vertex, jint vertexOffsetInBytes, jint strideInBytes, jobject vertices, jint verticesOffsetInBytes, jint numVertices) {
    return find(&((float *) ((jarray) vertex)->data)[vertexOffsetInBytes / 4], strideInBytes / 4, &((float *) ((jarray) vertices)->data)[verticesOffsetInBytes / 4], numVertices);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_find_java_nio_Buffer_int_int_java_nio_Buffer_int_int_float_R_long(jcontext ctx, jobject vertex, jint vertexOffsetInBytes, jint strideInBytes, jobject vertices, jint verticesOffsetInBytes, jint numVertices, jfloat epsilon) {
    return find(&((float *) ((java_nio_Buffer *) vertex)->F_address)[vertexOffsetInBytes / 4], strideInBytes / 4, &((float *) ((java_nio_Buffer *) vertices)->F_address)[verticesOffsetInBytes / 4], numVertices, epsilon);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_find_Array1_float_int_int_java_nio_Buffer_int_int_float_R_long(jcontext ctx, jobject vertex, jint vertexOffsetInBytes, jint strideInBytes, jobject vertices, jint verticesOffsetInBytes, jint numVertices, jfloat epsilon) {
    return find(&((float *) ((jarray) vertex)->data)[vertexOffsetInBytes / 4], strideInBytes / 4, &((float *) ((java_nio_Buffer *) vertices)->F_address)[verticesOffsetInBytes / 4], numVertices, epsilon);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_find_java_nio_Buffer_int_int_Array1_float_int_int_float_R_long(jcontext ctx, jobject vertex, jint vertexOffsetInBytes, jint strideInBytes, jobject vertices, jint verticesOffsetInBytes, jint numVertices, jfloat epsilon) {
    return find(&((float *) ((java_nio_Buffer *) vertex)->F_address)[vertexOffsetInBytes / 4], strideInBytes / 4, &((float *) ((jarray) vertices)->data)[verticesOffsetInBytes / 4], numVertices, epsilon);
}

jlong SM_com_badlogic_gdx_utils_BufferUtils_find_Array1_float_int_int_Array1_float_int_int_float_R_long(jcontext ctx, jobject vertex, jint vertexOffsetInBytes, jint strideInBytes, jobject vertices, jint verticesOffsetInBytes, jint numVertices, jfloat epsilon) {
    return find(&((float *) ((jarray) vertex)->data)[vertexOffsetInBytes / 4], strideInBytes / 4, &((float *) ((jarray) vertices)->data)[verticesOffsetInBytes / 4], numVertices, epsilon);
}

void SM_com_badlogic_gdx_math_Matrix4_mulVec_Array1_float_Array1_float_int_int_int(jcontext ctx, jobject mat, jobject vecs, jint offset, jint numVecs, jint stride) {
    auto vecPtr = (float *) ((jarray) vecs)->data + offset;
    for (int i = 0; i < numVecs; i++) {
        matrix4_mulVec((float *) ((jarray) mat)->data, vecPtr);
        vecPtr += stride;
    }
}

void SM_com_badlogic_gdx_math_Matrix4_prj_Array1_float_Array1_float_int_int_int(jcontext ctx, jobject mat, jobject vecs, jint offset, jint numVecs, jint stride) {
    auto vecPtr = (float *) ((jarray) vecs)->data + offset;
    for (int i = 0; i < numVecs; i++) {
        matrix4_proj((float *) ((jarray) mat)->data, vecPtr);
        vecPtr += stride;
    }
}

void SM_com_badlogic_gdx_math_Matrix4_rot_Array1_float_Array1_float_int_int_int(jcontext ctx, jobject mat, jobject vecs, jint offset, jint numVecs, jint stride) {
    auto vecPtr = (float *) ((jarray) vecs)->data + offset;
    for (int i = 0; i < numVecs; i++) {
        matrix4_rot((float *) ((jarray) mat)->data, vecPtr);
        vecPtr += stride;
    }
}

jobject SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_load_Array1_long_Array1_byte_int_int_R_java_nio_ByteBuffer(jcontext ctx, jobject nativeData, jobject buffer, jint offset, jint len) {
    auto pixmap = gdx2d_load((unsigned char *) ((jarray) buffer)->data + offset, len);
    if (!pixmap)
        return nullptr;
    auto pixelBuffer = gcAllocNative(ctx, &class_java_nio_ByteBuffer);
    init_java_nio_ByteBuffer_long_int_boolean(ctx, pixelBuffer, (jlong) pixmap->pixels, (jint) (pixmap->width * pixmap->height * gdx2d_bytes_per_pixel(pixmap->format)), false);
    pixelBuffer->gcMark = GC_MARK_START;
    auto nativeDataPtr = (jlong *) ((jarray) nativeData)->data;
    nativeDataPtr[0] = (jlong) pixmap;
    nativeDataPtr[1] = pixmap->width;
    nativeDataPtr[2] = pixmap->height;
    nativeDataPtr[3] = pixmap->format;
    return pixelBuffer;
}

jobject SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_loadByteBuffer_Array1_long_java_nio_ByteBuffer_int_int_R_java_nio_ByteBuffer(jcontext ctx, jobject nativeData, jobject buffer, jint offset, jint len) {
    auto pixmap = gdx2d_load((unsigned char *) ((java_nio_Buffer *) buffer)->F_address + offset, len);
    if (!pixmap)
        return nullptr;
    auto pixelBuffer = gcAllocNative(ctx, &class_java_nio_ByteBuffer);
    init_java_nio_ByteBuffer_long_int_boolean(ctx, pixelBuffer, (jlong) pixmap->pixels, (jint) (pixmap->width * pixmap->height * gdx2d_bytes_per_pixel(pixmap->format)), false);
    pixelBuffer->gcMark = GC_MARK_START;
    auto nativeDataPtr = (jlong *) ((jarray) nativeData)->data;
    nativeDataPtr[0] = (jlong) pixmap;
    nativeDataPtr[1] = pixmap->width;
    nativeDataPtr[2] = pixmap->height;
    nativeDataPtr[3] = pixmap->format;
    return pixelBuffer;
}

jobject SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_newPixmap_Array1_long_int_int_int_R_java_nio_ByteBuffer(jcontext ctx, jobject nativeData, jint width, jint height, jint format) {
    auto pixmap = gdx2d_new(width, height, format);
    if (!pixmap)
        return nullptr;
    auto pixelBuffer = gcAllocNative(ctx, &class_java_nio_ByteBuffer);
    init_java_nio_ByteBuffer_long_int_boolean(ctx, pixelBuffer, (jlong) pixmap->pixels, (jint) (pixmap->width * pixmap->height * gdx2d_bytes_per_pixel(pixmap->format)), false);
    pixelBuffer->gcMark = GC_MARK_START;
    auto nativeDataPtr = (jlong *) ((jarray) nativeData)->data;
    nativeDataPtr[0] = (jlong) pixmap;
    nativeDataPtr[1] = pixmap->width;
    nativeDataPtr[2] = pixmap->height;
    nativeDataPtr[3] = pixmap->format;
    return pixelBuffer;
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_free_long(jcontext ctx, jlong pixmap) {
    gdx2d_free((gdx2d_pixmap *) pixmap);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_clear_long_int(jcontext ctx, jlong pixmap, jint color) {
    gdx2d_clear((gdx2d_pixmap *) pixmap, color);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_setPixel_long_int_int_int(jcontext ctx, jlong pixmap, jint x, jint y, jint color) {
    gdx2d_set_pixel((gdx2d_pixmap *) pixmap, x, y, color);
}

jint SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_getPixel_long_int_int_R_int(jcontext ctx, jlong pixmap, jint x, jint y) {
    return (jint) gdx2d_get_pixel((gdx2d_pixmap *) pixmap, x, y);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_drawLine_long_int_int_int_int_int(jcontext ctx, jlong pixmap, jint x, jint y, jint x2, jint y2, jint color) {
    gdx2d_draw_line((gdx2d_pixmap *) pixmap, x, y, x2, y2, color);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_drawRect_long_int_int_int_int_int(jcontext ctx, jlong pixmap, jint x, jint y, jint width, jint height, jint color) {
    gdx2d_draw_rect((gdx2d_pixmap *) pixmap, x, y, width, height, color);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_drawCircle_long_int_int_int_int(jcontext ctx, jlong pixmap, jint x, jint y, jint radius, jint color) {
    gdx2d_draw_circle((gdx2d_pixmap *) pixmap, x, y, radius, color);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_fillRect_long_int_int_int_int_int(jcontext ctx, jlong pixmap, jint x, jint y, jint width, jint height, jint color) {
    gdx2d_fill_rect((gdx2d_pixmap *) pixmap, x, y, width, height, color);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_fillCircle_long_int_int_int_int(jcontext ctx, jlong pixmap, jint x, jint y, jint radius, jint color) {
    gdx2d_fill_circle((gdx2d_pixmap *) pixmap, x, y, radius, color);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_fillTriangle_long_int_int_int_int_int_int_int(jcontext ctx, jlong pixmap, jint x1, jint y1, jint x2, jint y2, jint x3, jint y3, jint color) {
    gdx2d_fill_triangle((gdx2d_pixmap *) pixmap, x1, y1, x2, y2, x3, y3, color);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_drawPixmap_long_long_int_int_int_int_int_int_int_int(jcontext ctx, jlong src, jlong dst, jint srcX, jint srcY, jint srcWidth, jint srcHeight, jint dstX, jint dstY, jint dstWidth, jint dstHeight) {
    gdx2d_draw_pixmap((gdx2d_pixmap *) src, (gdx2d_pixmap *) dst, srcX, srcY, srcWidth, srcHeight, dstX, dstY, dstWidth, dstHeight);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_setBlend_long_int(jcontext ctx, jlong src, jint blend) {
    gdx2d_set_blend((gdx2d_pixmap *) src, blend);
}

void SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_setScale_long_int(jcontext ctx, jlong src, jint scale) {
    gdx2d_set_scale((gdx2d_pixmap *) src, scale);
}

jobject SM_com_badlogic_gdx_graphics_g2d_Gdx2DPixmap_getFailureReason_R_java_lang_String(jcontext ctx) {
    return (jobject) stringFromNative(ctx, gdx2d_get_failure_reason());
}

jint SM_com_badlogic_gdx_graphics_glutils_ETC1_getCompressedDataSize_int_int_R_int(jcontext ctx, jint width, jint height) {
    return (jint) etc1_get_encoded_data_size(width, height);
}

void SM_com_badlogic_gdx_graphics_glutils_ETC1_formatHeader_java_nio_ByteBuffer_int_int_int(jcontext ctx, jobject header, jint offset, jint width, jint height) {
    etc1_pkm_format_header((etc1_byte *) ((java_nio_Buffer *) header)->F_address + offset, width, height);
}

jint SM_com_badlogic_gdx_graphics_glutils_ETC1_getWidthPKM_java_nio_ByteBuffer_int_R_int(jcontext ctx, jobject header, jint offset) {
    return (jint) etc1_pkm_get_width((etc1_byte *) ((java_nio_Buffer *) header)->F_address + offset);
}

jint SM_com_badlogic_gdx_graphics_glutils_ETC1_getHeightPKM_java_nio_ByteBuffer_int_R_int(jcontext ctx, jobject header, jint offset) {
    return (jint) etc1_pkm_get_height((etc1_byte *) ((java_nio_Buffer *) header)->F_address + offset);
}

jbool SM_com_badlogic_gdx_graphics_glutils_ETC1_isValidPKM_java_nio_ByteBuffer_int_R_boolean(jcontext ctx, jobject header, jint offset) {
    return etc1_pkm_is_valid((etc1_byte *) ((java_nio_Buffer *) header)->F_address + offset);
}

void SM_com_badlogic_gdx_graphics_glutils_ETC1_decodeImage_java_nio_ByteBuffer_int_java_nio_ByteBuffer_int_int_int_int(jcontext ctx, jobject compressedData, jint offset, jobject decodedData, jint offsetDec, jint width, jint height, jint pixelSize) {
    etc1_decode_image((etc1_byte *) ((java_nio_Buffer *) compressedData)->F_address + offset, (etc1_byte *) ((java_nio_Buffer *) decodedData)->F_address + offsetDec, width, height, pixelSize, width * pixelSize);
}

jobject SM_com_badlogic_gdx_graphics_glutils_ETC1_encodeImage_java_nio_ByteBuffer_int_int_int_int_R_java_nio_ByteBuffer(jcontext ctx, jobject imageData, jint offset, jint width, jint height, jint pixelSize) {
    auto compressedSize = etc1_get_encoded_data_size(width, height);
    auto compressedData = (etc1_byte *) malloc(compressedSize);
    etc1_encode_image((etc1_byte *) ((java_nio_Buffer *) imageData)->F_address + offset, width, height, pixelSize, width * pixelSize, compressedData);
    auto pixelBuffer = gcAllocNative(ctx, &class_java_nio_ByteBuffer);
    init_java_nio_ByteBuffer_long_int(ctx, pixelBuffer, (jlong) compressedData, (jint) compressedSize);
    pixelBuffer->gcMark = GC_MARK_START;
    return pixelBuffer;
}

jobject SM_com_badlogic_gdx_graphics_glutils_ETC1_encodeImagePKM_java_nio_ByteBuffer_int_int_int_int_R_java_nio_ByteBuffer(jcontext ctx, jobject imageData, jint offset, jint width, jint height, jint pixelSize) {
    auto compressedSize = etc1_get_encoded_data_size(width, height);
    auto compressed = (etc1_byte *) malloc(compressedSize + ETC_PKM_HEADER_SIZE);
    etc1_pkm_format_header(compressed, width, height);
    etc1_encode_image((etc1_byte *) ((java_nio_Buffer *) imageData)->F_address + offset, width, height, pixelSize, width * pixelSize, compressed + ETC_PKM_HEADER_SIZE);
    auto pixelBuffer = gcAllocNative(ctx, &class_java_nio_ByteBuffer);
    init_java_nio_ByteBuffer_long_int(ctx, pixelBuffer, (jlong) compressed, (jint) compressedSize);
    pixelBuffer->gcMark = GC_MARK_START;
    return pixelBuffer;
}

jint M_com_thelogicmaster_switchgdx_SwitchGraphics_getWidth_R_int(jcontext ctx, jobject self) {
    int width;
#ifdef __SWITCH__
//    eglQuerySurface(display, surface, EGL_WIDTH, &width);
    return 1280;
#else
    SDL_GetWindowSize(window, &width, nullptr);
#endif
    return width;
}

jint M_com_thelogicmaster_switchgdx_SwitchGraphics_getHeight_R_int(jcontext ctx, jobject self) {
    int height;
#ifdef __SWITCH__
//    eglQuerySurface(display, surface, EGL_HEIGHT, &height);
    return 720;
#else
    SDL_GetWindowSize(window, nullptr, &height);
#endif
    return height;
}

jint SM_com_thelogicmaster_switchgdx_SwitchControllerManager_getButtons_int_R_int(jcontext ctx, jint controller) {
#ifdef __SWITCH__
    auto &pad = controller == -1 ? combinedPad : pads[controller];
    return remapPadButtons(padGetButtons(&pad), padGetStyleSet(&pad));
#else
    return buttons;
#endif
}

void SM_com_thelogicmaster_switchgdx_SwitchControllerManager_getAxes_int_Array1_float(jcontext ctx, jint controller, jobject axes) {
    auto array = (float *) ((jarray) axes)->data;
#ifdef __SWITCH__
    const auto &pad = controller == -1 ? combinedPad : pads[controller];
    auto stickLeft = padGetStickPos(&pad, 0);
    auto stickRight = padGetStickPos(&pad, 1);
    array[0] = (float)stickLeft.x / JOYSTICK_MAX;
    array[1] = (float)stickLeft.y / JOYSTICK_MAX;
    array[2] = (float)stickRight.x / JOYSTICK_MAX;
    array[3] = (float)stickRight.y / JOYSTICK_MAX;
    remapPadAxes(array, padGetStyleSet(&pad));
    // Todo: Is inversion needed?
    array[1] *= -1;
    array[3] *= -1;
#else
    memcpy(array, joysticks, sizeof(joysticks));
#endif
}

jbool SM_com_thelogicmaster_switchgdx_SwitchControllerManager_isConnected_int_R_boolean(jcontext ctx, jint controller) {
#ifdef __SWITCH__
    return pads[controller].active_handheld or pads[controller].active_id_mask;
#else
    return controller == 0;
#endif
}

void SM_com_thelogicmaster_switchgdx_SwitchControllerManager_remapControllers_int_int_boolean_boolean(jcontext ctx, jint min, jint max, jbool dualJoy, jbool singleMode) {
#ifdef __SWITCH__
    HidLaControllerSupportArg arg;
    hidLaCreateControllerSupportArg(&arg);
    arg.hdr.player_count_min = min;
    arg.hdr.player_count_max = max;
    arg.hdr.enable_permit_joy_dual = dualJoy;
    arg.hdr.enable_single_mode = singleMode;
    hidLaShowControllerSupportForSystem(nullptr, &arg, false);
#endif
}

void SM_com_thelogicmaster_switchgdx_SwitchInput_getTouchData_Array1_int(jcontext ctx, jobject touchData) {
    memcpy((void *) ((jarray) touchData)->data, touches, sizeof(touches));
}

void M_com_thelogicmaster_switchgdx_SwitchInput_getTextInput_com_badlogic_gdx_Input$TextInputListener_java_lang_String_java_lang_String_java_lang_String_com_badlogic_gdx_Input$OnscreenKeyboardType
        (jcontext ctx, jobject self, jobject listener, jobject title, jobject text, jobject hint, jobject type) {
#ifdef __SWITCH__
    Result rc;
    SwkbdConfig kbd;
    char buffer[256];
    rc = swkbdCreate(&kbd, 0);
    if (rc)
        goto failed;
    swkbdConfigMakePresetDefault(&kbd);
    swkbdConfigSetHeaderText(&kbd, vm::getNativeString(title));
    swkbdConfigSetGuideText(&kbd, vm::getNativeString(text));
    swkbdConfigSetInitialText(&kbd, vm::getNativeString(hint));
    swkbdConfigSetStringLenMax(&kbd, sizeof(buffer) - 1);
    rc = swkbdShow(&kbd, buffer, sizeof(buffer));
    if (rc)
        goto failed;
    listener->M_input(vm::createString(buffer));
    return;
#elif defined(__WINRT__)
    goto failed;
#else
    auto input = tinyfd_inputBox(stringToNative(ctx, (jstring) title), stringToNative(ctx, (jstring) text), "");
    if (!input)
        goto failed;
    {
        auto textObj = (jobject) stringFromNative(ctx, input);
        textObj->gcMark = GC_MARK_NATIVE; // Todo: Should really just use a stack frame for parameters being passed around, this is error prone and leaky
        invokeInterface<func_com_badlogic_gdx_Input$TextInputListener_input_java_lang_String, &class_com_badlogic_gdx_Input$TextInputListener, INDEX_com_badlogic_gdx_Input$TextInputListener_input_java_lang_String>(ctx, listener, textObj);
        textObj->gcMark = GC_MARK_START;
        return;
    }
#endif
    failed:
    invokeInterface<func_com_badlogic_gdx_Input$TextInputListener_canceled, &class_com_badlogic_gdx_Input$TextInputListener, INDEX_com_badlogic_gdx_Input$TextInputListener_canceled>(ctx, listener);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glActiveTexture_int(jcontext ctx, jobject self, jint texture) {
    glActiveTexture(texture);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBindTexture_int_int(jcontext ctx, jobject self, jint target, jint texture) {
    glBindTexture(target, texture);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBlendFunc_int_int(jcontext ctx, jobject self, jint sfactor, jint dfactor) {
    glBlendFunc(sfactor, dfactor);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glClear_int(jcontext ctx, jobject self, jint mask) {
    glClear(mask);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glClearColor_float_float_float_float(jcontext ctx, jobject self, jfloat red, jfloat green, jfloat blue, jfloat alpha) {
    glClearColor(red, green, blue, alpha);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glClearDepthf_float(jcontext ctx, jobject self, jfloat depth) {
    glClearDepthf(depth);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glClearStencil_int(jcontext ctx, jobject self, jint s) {
    glClearStencil(s);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glColorMask_boolean_boolean_boolean_boolean(jcontext ctx, jobject self, jbool red, jbool green, jbool blue, jbool alpha) {
    glColorMask(red, green, blue, alpha);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glCompressedTexImage2D_int_int_int_int_int_int_int_java_nio_Buffer(jcontext ctx, jobject self, jint target, jint level, jint internalformat, jint width, jint height, jint border, jint imageSize, jobject data) {
    glCompressedTexImage2D(target, level, internalformat, width, height, border, imageSize, (void *) getBufferAddress(ctx, (java_nio_Buffer *) data));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glCompressedTexSubImage2D_int_int_int_int_int_int_int_int_java_nio_Buffer(jcontext ctx, jobject self, jint target, jint level, jint xoffset, jint yoffset, jint width, jint height, jint format, jint imageSize, jobject data) {
    glCompressedTexSubImage2D(target, level, xoffset, yoffset, width, height, format, imageSize, (void *) getBufferAddress(ctx, (java_nio_Buffer *) data));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glCopyTexImage2D_int_int_int_int_int_int_int_int(jcontext ctx, jobject self, jint target, jint level, jint internalformat, jint x, jint y, jint width, jint height, jint border) {
    glCopyTexImage2D(target, level, internalformat, x, y, width, height, border);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glCopyTexSubImage2D_int_int_int_int_int_int_int_int(jcontext ctx, jobject self, jint target, jint level, jint xoffset, jint yoffset, jint x, jint y, jint width, jint height) {
    glCopyTexSubImage2D(target, level, xoffset, yoffset, x, y, width, height);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glCullFace_int(jcontext ctx, jobject self, jint mode) {
    glCullFace(mode);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteTextures_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint n, jobject textures) {
    glDeleteTextures(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) textures));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteTexture_int(jcontext ctx, jobject self, jint texture) {
    glDeleteTextures(1, (GLuint *) &texture);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDepthFunc_int(jcontext ctx, jobject self, jint func) {
    glDepthFunc(func);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDepthMask_boolean(jcontext ctx, jobject self, jbool flag) {
    glDepthMask(flag);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDepthRangef_float_float(jcontext ctx, jobject self, jfloat zNear, jfloat zFar) {
    glDepthRangef(zNear, zFar);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDisable_int(jcontext ctx, jobject self, jint cap) {
    glDisable(cap);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDrawArrays_int_int_int(jcontext ctx, jobject self, jint mode, jint first, jint count) {
    glDrawArrays(mode, first, count);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDrawElements_int_int_int_java_nio_Buffer(jcontext ctx, jobject self, jint mode, jint count, jint type, jobject indices) {
    glDrawElements(mode, count, type, (void *) getBufferAddress(ctx, (java_nio_Buffer *) indices));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glEnable_int(jcontext ctx, jobject self, jint cap) {
    glEnable(cap);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glFinish(jcontext ctx, jobject self) {
    glFinish();
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glFlush(jcontext ctx, jobject self) {
    glFlush();
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glFrontFace_int(jcontext ctx, jobject self, jint mode) {
    glFrontFace(mode);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGenTextures_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint n, jobject textures) {
    glGenTextures(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) textures));
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glGenTexture_R_int(jcontext ctx, jobject self) {
    GLuint texture;
    glGenTextures(1, &texture);
    return (jint) texture;
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glGetError_R_int(jcontext ctx, jobject self) {
    return (jint) glGetError();
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetIntegerv_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint pname, jobject params) {
    glGetIntegerv(pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

jobject M_com_thelogicmaster_switchgdx_SwitchGL_glGetString_int_R_java_lang_String(jcontext ctx, jobject self, jint name) {
    return (jobject) stringFromNative(ctx, (char *) glGetString(name));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glHint_int_int(jcontext ctx, jobject self, jint target, jint mode) {
    glHint(target, mode);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glLineWidth_float(jcontext ctx, jobject self, jfloat width) {
    glLineWidth(width);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glPixelStorei_int_int(jcontext ctx, jobject self, jint pname, jint param) {
    glPixelStorei(pname, param);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glPolygonOffset_float_float(jcontext ctx, jobject self, jfloat factor, jfloat units) {
    glPolygonOffset(factor, units);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glReadPixels_int_int_int_int_int_int_java_nio_Buffer(jcontext ctx, jobject self, jint x, jint y, jint width, jint height, jint format, jint type, jobject pixels) {
    glReadPixels(x, y, width, height, format, type, (void *) getBufferAddress(ctx, (java_nio_Buffer *) pixels));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glScissor_int_int_int_int(jcontext ctx, jobject self, jint x, jint y, jint width, jint height) {
    glScissor(x, y, width, height);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glStencilFunc_int_int_int(jcontext ctx, jobject self, jint func, jint ref, jint mask) {
    glStencilFunc(func, ref, mask);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glStencilMask_int(jcontext ctx, jobject self, jint mask) {
    glStencilMask(mask);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glStencilOp_int_int_int(jcontext ctx, jobject self, jint fail, jint zfail, jint zpass) {
    glStencilOp(fail, zfail, zpass);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glTexImage2D_int_int_int_int_int_int_int_int_java_nio_Buffer(jcontext ctx, jobject self, jint target, jint level, jint internalformat, jint width, jint height, jint border, jint format, jint type, jobject pixels) {
    glTexImage2D(target, level, internalformat, width, height, border, format, type, (void *) getBufferAddress(ctx, (java_nio_Buffer *) pixels));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glTexParameterf_int_int_float(jcontext ctx, jobject self, jint target, jint pname, jfloat param) {
    glTexParameterf(target, pname, param);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glTexSubImage2D_int_int_int_int_int_int_int_int_java_nio_Buffer(jcontext ctx, jobject self, jint target, jint level, jint xoffset, jint yoffset, jint width, jint height, jint format, jint type, jobject pixels) {
    glTexSubImage2D(target, level, xoffset, yoffset, width, height, format, type, (void *) getBufferAddress(ctx, (java_nio_Buffer *) pixels));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glViewport_int_int_int_int(jcontext ctx, jobject self, jint x, jint y, jint width, jint height) {
    glViewport(x, y, width, height);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glAttachShader_int_int(jcontext ctx, jobject self, jint program, jint shader) {
    glAttachShader(program, shader);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBindAttribLocation_int_int_java_lang_String(jcontext ctx, jobject self, jint program, jint index, jobject name) {
    glBindAttribLocation(program, index, stringToNative(ctx, (jstring) name));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBindBuffer_int_int(jcontext ctx, jobject self, jint target, jint buffer) {
    glBindBuffer(target, buffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBindFramebuffer_int_int(jcontext ctx, jobject self, jint target, jint framebuffer) {
    glBindFramebuffer(target, framebuffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBindRenderbuffer_int_int(jcontext ctx, jobject self, jint target, jint renderbuffer) {
    glBindRenderbuffer(target, renderbuffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBlendColor_float_float_float_float(jcontext ctx, jobject self, jfloat red, jfloat green, jfloat blue, jfloat alpha) {
    glBlendColor(red, green, blue, alpha);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBlendEquation_int(jcontext ctx, jobject self, jint mode) {
    glBlendEquation(mode);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBlendEquationSeparate_int_int(jcontext ctx, jobject self, jint modeRGB, jint modeAlpha) {
    glBlendEquationSeparate(modeRGB, modeAlpha);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBlendFuncSeparate_int_int_int_int(jcontext ctx, jobject self, jint srcRGB, jint dstRGB, jint srcAlpha, jint dstAlpha) {
    glBlendFuncSeparate(srcRGB, dstRGB, srcAlpha, dstAlpha);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBufferData_int_int_java_nio_Buffer_int(jcontext ctx, jobject self, jint target, jint size, jobject buffer, jint usage) {
    glBufferData(target, size, (void *) getBufferAddress(ctx, (java_nio_Buffer *) buffer), usage);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glBufferSubData_int_int_int_java_nio_Buffer(jcontext ctx, jobject self, jint target, jint offset, jint size, jobject data) {
    glBufferSubData(target, offset, size, (void *) getBufferAddress(ctx, (java_nio_Buffer *) data));
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glCheckFramebufferStatus_int_R_int(jcontext ctx, jobject self, jint target) {
    return (jint) glCheckFramebufferStatus(target);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glCompileShader_int(jcontext ctx, jobject self, jint shader) {
    glCompileShader(shader);
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glCreateProgram_R_int(jcontext ctx, jobject self) {
    return (int) glCreateProgram();
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glCreateShader_int_R_int(jcontext ctx, jobject self, jint type) {
    return (int) glCreateShader(type);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteBuffer_int(jcontext ctx, jobject self, jint buffer) {
    glDeleteBuffers(1, (GLuint *) &buffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteBuffers_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint n, jobject buffers) {
    glDeleteBuffers(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) buffers));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteFramebuffer_int(jcontext ctx, jobject self, jint framebuffer) {
    glDeleteFramebuffers(1, (GLuint *) &framebuffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteFramebuffers_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint n, jobject framebuffers) {
    glDeleteFramebuffers(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) framebuffers));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteProgram_int(jcontext ctx, jobject self, jint program) {
    glDeleteProgram(program);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteRenderbuffer_int(jcontext ctx, jobject self, jint renderbuffer) {
    glDeleteRenderbuffers(1, (GLuint *) &renderbuffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteRenderbuffers_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint n, jobject renderbuffers) {
    glDeleteRenderbuffers(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) renderbuffers));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDeleteShader_int(jcontext ctx, jobject self, jint shader) {
    glDeleteShader(shader);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDetachShader_int_int(jcontext ctx, jobject self, jint program, jint shader) {
    glDetachShader(program, shader);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDisableVertexAttribArray_int(jcontext ctx, jobject self, jint index) {
    glDisableVertexAttribArray(index);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glDrawElements_int_int_int_int(jcontext ctx, jobject self, jint mode, jint count, jint type, jint indices) {
    glDrawElements(mode, count, type, (void *) (jlong) indices);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glEnableVertexAttribArray_int(jcontext ctx, jobject self, jint index) {
    glEnableVertexAttribArray(index);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glFramebufferRenderbuffer_int_int_int_int(jcontext ctx, jobject self, jint target, jint attachment, jint renderbuffertarget, jint renderbuffer) {
    glFramebufferRenderbuffer(target, attachment, renderbuffertarget, renderbuffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glFramebufferTexture2D_int_int_int_int_int(jcontext ctx, jobject self, jint target, jint attachment, jint textarget, jint texture, jint level) {
    glFramebufferTexture2D(target, attachment, textarget, texture, level);
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glGenBuffer_R_int(jcontext ctx, jobject self) {
    GLuint buffer;
    glGenBuffers(1, &buffer);
    return (jint) buffer;
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGenBuffers_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint n, jobject buffers) {
    glGenBuffers(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) buffers));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGenerateMipmap_int(jcontext ctx, jobject self, jint target) {
    glGenerateMipmap(target);
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glGenFramebuffer_R_int(jcontext ctx, jobject self) {
    GLuint buffer;
    glGenFramebuffers(1, &buffer);
    return (jint) buffer;
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGenFramebuffers_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint n, jobject framebuffers) {
    glGenFramebuffers(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) framebuffers));
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glGenRenderbuffer_R_int(jcontext ctx, jobject self) {
    GLuint buffer;
    glGenRenderbuffers(1, &buffer);
    return (jint) buffer;
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGenRenderbuffers_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint n, jobject renderbuffers) {
    glGenRenderbuffers(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) renderbuffers));
}

jobject M_com_thelogicmaster_switchgdx_SwitchGL_glGetActiveAttrib_int_int_java_nio_IntBuffer_java_nio_IntBuffer_R_java_lang_String(jcontext ctx, jobject self, jint program, jint index, jobject size, jobject type) {
    char buffer[64];
    glGetActiveAttrib(program, index, 63, nullptr, (GLint *) getBufferAddress((java_nio_IntBuffer *) size), (GLenum *) getBufferAddress((java_nio_IntBuffer *) type), buffer);
    return (jobject) stringFromNative(ctx, buffer);
}

jobject M_com_thelogicmaster_switchgdx_SwitchGL_glGetActiveUniform_int_int_java_nio_IntBuffer_java_nio_IntBuffer_R_java_lang_String(jcontext ctx, jobject self, jint program, jint index, jobject size, jobject type) {
    char buffer[64];
    glGetActiveUniform(program, index, 63, nullptr, (GLint *) getBufferAddress((java_nio_IntBuffer *) size), (GLenum *) getBufferAddress((java_nio_IntBuffer *) type), buffer);
    return (jobject) stringFromNative(ctx, buffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetAttachedShaders_int_int_java_nio_Buffer_java_nio_IntBuffer(jcontext ctx, jobject self, jint program, jint maxcount, jobject count, jobject shaders) {
    glGetAttachedShaders(program, maxcount, (GLsizei *) getBufferAddress(ctx, (java_nio_Buffer *) count), (GLuint *) getBufferAddress((java_nio_IntBuffer *) shaders));
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glGetAttribLocation_int_java_lang_String_R_int(jcontext ctx, jobject self, jint program, jobject name) {
    return glGetAttribLocation(program, stringToNative(ctx, (jstring) name));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetBooleanv_int_java_nio_Buffer(jcontext ctx, jobject self, jint pname, jobject params) {
    glGetBooleanv(pname, (GLboolean *) getBufferAddress(ctx, (java_nio_Buffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetBufferParameteriv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint target, jint pname, jobject params) {
    glGetBufferParameteriv(target, pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetFloatv_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint pname, jobject params) {
    glGetFloatv(pname, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetFramebufferAttachmentParameteriv_int_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint target, jint attachment, jint pname, jobject params) {
    glGetFramebufferAttachmentParameteriv(target, attachment, pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetProgramiv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint program, jint pname, jobject params) {
    glGetProgramiv(program, pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

jobject M_com_thelogicmaster_switchgdx_SwitchGL_glGetProgramInfoLog_int_R_java_lang_String(jcontext ctx, jobject self, jint program) {
    char buffer[128];
    glGetProgramInfoLog(program, 127, nullptr, buffer);
    return (jobject) stringFromNative(ctx, buffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetRenderbufferParameteriv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint target, jint pname, jobject params) {
    glGetRenderbufferParameteriv(target, pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetShaderiv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint shader, jint pname, jobject params) {
    glGetShaderiv(shader, pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

jobject M_com_thelogicmaster_switchgdx_SwitchGL_glGetShaderInfoLog_int_R_java_lang_String(jcontext ctx, jobject self, jint program) {
    char buffer[128];
    glGetShaderInfoLog(program, 127, nullptr, buffer);
    return (jobject) stringFromNative(ctx, buffer);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetShaderPrecisionFormat_int_int_java_nio_IntBuffer_java_nio_IntBuffer(jcontext ctx, jobject self, jint shadertype, jint precisiontype, jobject range, jobject precision) {
    glGetShaderPrecisionFormat(shadertype, precisiontype, (GLint *) getBufferAddress((java_nio_IntBuffer *) range), (GLint *) getBufferAddress((java_nio_IntBuffer *) precision));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetTexParameterfv_int_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint target, jint pname, jobject params) {
    glGetTexParameterfv(target, pname, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetTexParameteriv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint target, jint pname, jobject params) {
    glGetTexParameteriv(target, pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetUniformfv_int_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint program, jint location, jobject params) {
    glGetUniformfv(program, location, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetUniformiv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint program, jint location, jobject params) {
    glGetUniformiv(program, location, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

jint M_com_thelogicmaster_switchgdx_SwitchGL_glGetUniformLocation_int_java_lang_String_R_int(jcontext ctx, jobject self, jint program, jobject name) {
    return glGetUniformLocation(program, stringToNative(ctx, (jstring) name));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetVertexAttribfv_int_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint index, jint pname, jobject params) {
    glGetVertexAttribfv(index, pname, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetVertexAttribiv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint index, jint pname, jobject params) {
    glGetVertexAttribiv(index, pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glGetVertexAttribPointerv_int_int_java_nio_Buffer(jcontext ctx, jobject self, jint index, jint pname, jobject pointer) {
    glGetVertexAttribPointerv(index, pname, (void **) getBufferAddress(ctx, (java_nio_Buffer *) pointer));
}

jbool M_com_thelogicmaster_switchgdx_SwitchGL_glIsBuffer_int_R_boolean(jcontext ctx, jobject self, jint buffer) {
    return glIsBuffer(buffer);
}

jbool M_com_thelogicmaster_switchgdx_SwitchGL_glIsEnabled_int_R_boolean(jcontext ctx, jobject self, jint cap) {
    return glIsEnabled(cap);
}

jbool M_com_thelogicmaster_switchgdx_SwitchGL_glIsFramebuffer_int_R_boolean(jcontext ctx, jobject self, jint framebuffer) {
    return glIsFramebuffer(framebuffer);
}

jbool M_com_thelogicmaster_switchgdx_SwitchGL_glIsProgram_int_R_boolean(jcontext ctx, jobject self, jint program) {
    return glIsProgram(program);
}

jbool M_com_thelogicmaster_switchgdx_SwitchGL_glIsRenderbuffer_int_R_boolean(jcontext ctx, jobject self, jint renderbuffer) {
    return glIsRenderbuffer(renderbuffer);
}

jbool M_com_thelogicmaster_switchgdx_SwitchGL_glIsShader_int_R_boolean(jcontext ctx, jobject self, jint shader) {
    return glIsShader(shader);
}

jbool M_com_thelogicmaster_switchgdx_SwitchGL_glIsTexture_int_R_boolean(jcontext ctx, jobject self, jint texture) {
    return glIsTexture(texture);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glLinkProgram_int(jcontext ctx, jobject self, jint program) {
    glLinkProgram(program);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glReleaseShaderCompiler(jcontext ctx, jobject self) {
    glReleaseShaderCompiler();
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glRenderbufferStorage_int_int_int_int(jcontext ctx, jobject self, jint target, jint internalformat, jint width, jint height) {
    glRenderbufferStorage(target, internalformat, width, height);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glSampleCoverage_float_boolean(jcontext ctx, jobject self, jfloat value, jbool invert) {
    glSampleCoverage(value, invert);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glShaderBinary_int_java_nio_IntBuffer_int_java_nio_Buffer_int(jcontext ctx, jobject self, jint n, jobject shaders, jint binaryformat, jobject binary, jint length) {
    glShaderBinary(n, (GLuint *) getBufferAddress((java_nio_IntBuffer *) shaders), binaryformat, (void *) getBufferAddress(ctx, (java_nio_Buffer *) binary), length);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glShaderSource_int_java_lang_String(jcontext ctx, jobject self, jint shader, jobject sourceObject) {
    auto source = stringToNative(ctx, (jstring) sourceObject);
    glShaderSource(shader, 1, &source, nullptr);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glStencilFuncSeparate_int_int_int_int(jcontext ctx, jobject self, jint face, jint func, jint ref, jint mask) {
    glStencilFuncSeparate(face, func, ref, mask);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glStencilMaskSeparate_int_int(jcontext ctx, jobject self, jint face, jint mask) {
    glStencilMaskSeparate(face, mask);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glStencilOpSeparate_int_int_int_int(jcontext ctx, jobject self, jint face, jint fail, jint zfail, jint zpass) {
    glStencilOpSeparate(face, fail, zfail, zpass);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glTexParameterfv_int_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint target, jint pname, jobject params) {
    glTexParameterfv(target, pname, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glTexParameteri_int_int_int(jcontext ctx, jobject self, jint target, jint pname, jint param) {
    glTexParameteri(target, pname, param);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glTexParameteriv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint target, jint pname, jobject params) {
    glTexParameteriv(target, pname, (GLint *) getBufferAddress((java_nio_IntBuffer *) params));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform1f_int_float(jcontext ctx, jobject self, jint location, jfloat x) {
    glUniform1f(location, x);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform1fv_int_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint location, jint count, jobject v) {
    glUniform1fv(location, count, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) v));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform1fv_int_int_Array1_float_int(jcontext ctx, jobject self, jint location, jint count, jobject v, jint offset) {
    glUniform1fv(location, count, (GLfloat *) ((jarray) v)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform1i_int_int(jcontext ctx, jobject self, jint location, jint x) {
    glUniform1i(location, x);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform1iv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint location, jint count, jobject v) {
    glUniform1iv(location, count, (GLint *) getBufferAddress((java_nio_IntBuffer *) v));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform1iv_int_int_Array1_int_int(jcontext ctx, jobject self, jint location, jint count, jobject v, jint offset) {
    glUniform1iv(location, count, (GLint *) ((jarray) v)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform2f_int_float_float(jcontext ctx, jobject self, jint location, jfloat x, jfloat y) {
    glUniform2f(location, x, y);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform2fv_int_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint location, jint count, jobject v) {
    glUniform2fv(location, count, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) v));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform2fv_int_int_Array1_float_int(jcontext ctx, jobject self, jint location, jint count, jobject v, jint offset) {
    glUniform2fv(location, count, (GLfloat *) ((jarray) v)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform2i_int_int_int(jcontext ctx, jobject self, jint location, jint x, jint y) {
    glUniform2i(location, x, y);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform2iv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint location, jint count, jobject v) {
    glUniform2iv(location, count, (GLint *) getBufferAddress((java_nio_IntBuffer *) v));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform2iv_int_int_Array1_int_int(jcontext ctx, jobject self, jint location, jint count, jobject v, jint offset) {
    glUniform2iv(location, count, (GLint *) ((jarray) v)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform3f_int_float_float_float(jcontext ctx, jobject self, jint location, jfloat x, jfloat y, jfloat z) {
    glUniform3f(location, x, y, z);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform3fv_int_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint location, jint count, jobject v) {
    glUniform3fv(location, count, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) v));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform3fv_int_int_Array1_float_int(jcontext ctx, jobject self, jint location, jint count, jobject v, jint offset) {
    glUniform3fv(location, count, (GLfloat *) ((jarray) v)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform3i_int_int_int_int(jcontext ctx, jobject self, jint location, jint x, jint y, jint z) {
    glUniform3i(location, x, y, z);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform3iv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint location, jint count, jobject v) {
    glUniform3iv(location, count, (GLint *) getBufferAddress((java_nio_IntBuffer *) v));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform3iv_int_int_Array1_int_int(jcontext ctx, jobject self, jint location, jint count, jobject v, jint offset) {
    glUniform3iv(location, count, (GLint *) ((jarray) v)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform4f_int_float_float_float_float(jcontext ctx, jobject self, jint location, jfloat x, jfloat y, jfloat z, jfloat w) {
    glUniform4f(location, x, y, z, w);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform4fv_int_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint location, jint count, jobject v) {
    glUniform4fv(location, count, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) v));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform4fv_int_int_Array1_float_int(jcontext ctx, jobject self, jint location, jint count, jobject v, jint offset) {
    glUniform4fv(location, count, (GLfloat *) ((jarray) v)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform4i_int_int_int_int_int(jcontext ctx, jobject self, jint location, jint x, jint y, jint z, jint w) {
    glUniform4i(location, x, y, z, w);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform4iv_int_int_java_nio_IntBuffer(jcontext ctx, jobject self, jint location, jint count, jobject v) {
    glUniform4iv(location, count, (GLint *) getBufferAddress((java_nio_IntBuffer *) v));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniform4iv_int_int_Array1_int_int(jcontext ctx, jobject self, jint location, jint count, jobject v, jint offset) {
    glUniform4iv(location, count, (GLint *) ((jarray) v)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniformMatrix2fv_int_int_boolean_java_nio_FloatBuffer(jcontext ctx, jobject self, jint location, jint count, jbool transpose, jobject value) {
    glUniformMatrix2fv(location, count, transpose, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) value));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniformMatrix2fv_int_int_boolean_Array1_float_int(jcontext ctx, jobject self, jint location, jint count, jbool transpose, jobject value, jint offset) {
    glUniformMatrix2fv(location, count, transpose, (GLfloat *) ((jarray) value)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniformMatrix3fv_int_int_boolean_java_nio_FloatBuffer(jcontext ctx, jobject self, jint location, jint count, jbool transpose, jobject value) {
    glUniformMatrix3fv(location, count, transpose, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) value));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniformMatrix3fv_int_int_boolean_Array1_float_int(jcontext ctx, jobject self, jint location, jint count, jbool transpose, jobject value, jint offset) {
    glUniformMatrix3fv(location, count, transpose, (GLfloat *) ((jarray) value)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniformMatrix4fv_int_int_boolean_java_nio_FloatBuffer(jcontext ctx, jobject self, jint location, jint count, jbool transpose, jobject value) {
    glUniformMatrix4fv(location, count, transpose, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) value));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUniformMatrix4fv_int_int_boolean_Array1_float_int(jcontext ctx, jobject self, jint location, jint count, jbool transpose, jobject value, jint offset) {
    glUniformMatrix4fv(location, count, transpose, (GLfloat *) ((jarray) value)->data + offset);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glUseProgram_int(jcontext ctx, jobject self, jint program) {
    glUseProgram(program);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glValidateProgram_int(jcontext ctx, jobject self, jint program) {
    glValidateProgram(program);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttrib1f_int_float(jcontext ctx, jobject self, jint indx, jfloat x) {
    glVertexAttrib1f(indx, x);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttrib1fv_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint indx, jobject values) {
    glVertexAttrib1fv(indx, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) values));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttrib2f_int_float_float(jcontext ctx, jobject self, jint indx, jfloat x, jfloat y) {
    glVertexAttrib2f(indx, x, y);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttrib2fv_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint indx, jobject values) {
    glVertexAttrib2fv(indx, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) values));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttrib3f_int_float_float_float(jcontext ctx, jobject self, jint indx, jfloat x, jfloat y, jfloat z) {
    glVertexAttrib3f(indx, x, y, z);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttrib3fv_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint indx, jobject values) {
    glVertexAttrib3fv(indx, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) values));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttrib4f_int_float_float_float_float(jcontext ctx, jobject self, jint indx, jfloat x, jfloat y, jfloat z, jfloat w) {
    glVertexAttrib4f(indx, x, y, z, w);
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttrib4fv_int_java_nio_FloatBuffer(jcontext ctx, jobject self, jint indx, jobject values) {
    glVertexAttrib4fv(indx, (GLfloat *) getBufferAddress((java_nio_FloatBuffer *) values));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttribPointer_int_int_int_boolean_int_java_nio_Buffer(jcontext ctx, jobject self, jint index, jint size, jint type, jbool normalized, jint stride, jobject ptr) {
    glVertexAttribPointer(index, size, type, normalized, stride, getBufferAddress(ctx, (java_nio_Buffer *) ptr));
}

void M_com_thelogicmaster_switchgdx_SwitchGL_glVertexAttribPointer_int_int_int_boolean_int_int(jcontext ctx, jobject self, jint index, jint size, jint type, jbool normalized, jint stride, jint ptr) {
    glVertexAttribPointer(index, size, type, normalized, stride, (void *) (jlong) ptr);
}
