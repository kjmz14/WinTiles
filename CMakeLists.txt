cmake_minimum_required(VERSION 3.25)

project(WinTiles VERSION 1.0.0)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

add_executable(
        WinTiles WIN32
        res/res.h # Resources header
        res/WinTiles.exe.manifest # Manifest
        # Sources
        src/tiles/tiles.cpp src/tiles/tiles.hpp
        src/tiles/highscores/highscores.cpp src/tiles/highscores/highscores.hpp
        src/main.cpp src/main.hpp
        src/windowproc.cpp src/windowproc.hpp
        src/paint.cpp src/paint.hpp
        src/command.cpp src/command.hpp
        src/key.cpp src/key.hpp
        src/strings.cpp src/strings.hpp
        src/dlgproc.cpp src/dlgproc.hpp
        src/save.cpp src/save.hpp
        src/extra.cpp src/extra.hpp
)

# Resources
target_sources(
        WinTiles PRIVATE
        res/version.rc
        res/accel.rc
        res/menu.rc
        res/dialogs.rc
        res/strings.rc
        res/icons/icons.rc
)

configure_file(
        res/wtversion.in.h
        ${CMAKE_BINARY_DIR}/wtversion.h
        @ONLY
)

target_include_directories(
        WinTiles PRIVATE
        ${CMAKE_BINARY_DIR}
)

add_compile_definitions(
        UNICODE # Macros use Unicode (W) versions of Windows API functions
        WIN32_LEAN_AND_MEAN # Strips windows.h from rarely used parts
)

# Adding compiler flags
# Debug builds aren't stripped and are compiled without optimizations

if (MSVC) # MSVC, Clang-cl

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /W4 /MP")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /WX /O2 /GL")

    if (MSVC_VERSION GREATER_EQUAL 1920) # Visual Studio 2019 or newer
        set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} /Ob3")
    endif ()

    set(CMAKE_EXE_LINKER_FLAGS_RELEASE "${CMAKE_EXE_LINKER_FLAGS} /LTCG /SUBSYSTEM:WINDOWS /OSVERSION:6.1")

else () # MinGW

    target_sources(
            WinTiles PRIVATE
            res/mingw_manifest.rc
    )

    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -municode -mwindows -Wl,--major-subsystem-version,6,--minor-subsystem-version,1")

    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -Werror -O3 -msse4.2 -s -static")

endif ()
