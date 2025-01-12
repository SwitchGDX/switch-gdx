if not defined DevEnvDir (
    call "C:\Program Files\Microsoft Visual Studio\2022\Community\VC\Auxiliary\Build\vcvars64.bat"
)
cmake -B build-uwp -G "Visual Studio 17 2022" -S . -DVCPKG_ROOT="C:\Program Files\Microsoft Visual Studio\2022\Community\VC\vcpkg" -DCMAKE_SYSTEM_NAME=WindowsStore -DCMAKE_SYSTEM_VERSION="10.0"
cmake --open build-uwp
