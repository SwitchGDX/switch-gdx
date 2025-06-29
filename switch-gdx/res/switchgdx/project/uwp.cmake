cmake_minimum_required(VERSION 3.10)
project(SwitchGDX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_C_STANDARD 11)

include_directories(src src/soloud) src/switchgdx/bullet/bullet src/switchgdx/bullet/custom src/switchgdx/bullet/extras/Serialize src/switchgdx/bullet/extras)
file(GLOB_RECURSE SRCS src/*.cpp src/*.c)
add_executable(SwitchGDX WIN32 ${SRCS})

target_compile_options(SwitchGDX PRIVATE "-await" "/Zc:preprocessor")
target_compile_definitions(SwitchGDX PRIVATE UNICODE WITH_SDL2_STATIC NOJNI ASMJIT_NO_JIT BT_NO_PROFILE BT_USE_INVERSE_DYNAMICS_WITH_BULLET2)

set(CMAKE_SYSTEM_NAME "WindowsStore" CACHE INTERNAL "" FORCE )

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT SwitchGDX)

if (NOT EXISTS ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)
    message(FATAL_ERROR "Cannot find vcpkg toolchain file at ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake. Please check the VCPKG_ROOT variable")
endif()

set(CMAKE_TOOLCHAIN_FILE ${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE PATH "")
include(${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake)

find_package(unofficial-angle REQUIRED)
find_package(SDL2 REQUIRED)
find_package(SDL2_mixer REQUIRED)

find_package(ZLIB REQUIRED)
find_package(Threads REQUIRED)
find_package(libffi REQUIRED)
find_package(Freetype REQUIRED)
find_package(CURL REQUIRED)

if(THREADS_HAVE_PTHREAD_ARG)
  target_compile_options(SwitchGDX PUBLIC "-pthread")
endif()

add_definitions(-DNOJNI -D__WINRT__)

if(CMAKE_THREAD_LIBS_INIT)
  target_link_libraries(SwitchGDX PRIVATE "${CMAKE_THREAD_LIBS_INIT}")
endif()
if(WIN32)
  target_link_libraries(SwitchGDX PRIVATE wsock32 ws2_32)
endif()

target_link_libraries(SwitchGDX PRIVATE unofficial::angle::libEGL unofficial::angle::libGLESv2 SDL2::SDL2 SDL2_mixer::SDL2_mixer ZLIB::ZLIB CURL::libcurl libffi ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-uwp/lib/zzip.lib ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-uwp/lib/freetype.lib ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-uwp/lib/mpg123.lib)

target_link_libraries(SwitchGDX PRIVATE WindowsApp.lib rpcrt4.lib onecoreuap.lib kernel32.lib)
set_target_properties(SwitchGDX PROPERTIES VS_GLOBAL_MinimalCoreWin "true")

FILE(GLOB DLLS ${CMAKE_BINARY_DIR}/vcpkg_installed/x64-uwp/bin/*.dll)
list(APPEND DLLS
	${CMAKE_BINARY_DIR}/vcpkg_installed/x64-uwp/debug/bin/SDL2d.dll
	${CMAKE_BINARY_DIR}/vcpkg_installed/x64-uwp/debug/bin/SDL2_mixerd.dll
	${CMAKE_BINARY_DIR}/vcpkg_installed/x64-uwp/debug/bin/zlibd1.dll
	${CMAKE_BINARY_DIR}/vcpkg_installed/x64-uwp/debug/bin/libcurl-d.dll
)

target_sources(SwitchGDX PRIVATE ${DLLS})
set_property(SOURCE ${DLLS} PROPERTY VS_DEPLOYMENT_CONTENT 1)
set_property(SOURCE ${DLLS} PROPERTY VS_DEPLOYMENT_LOCATION ".")

target_compile_options(SwitchGDX PRIVATE -permissive-)
target_compile_options(SwitchGDX PRIVATE -W0 -sdl-)
target_compile_options(SwitchGDX PRIVATE -EHsc)

add_custom_command(TARGET SwitchGDX POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_SOURCE_DIR}/romfs "$(LayoutDir)"
)
