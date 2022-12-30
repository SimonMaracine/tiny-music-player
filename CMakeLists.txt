cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(EXECUTABLE "tiny-music-player")

set(ALSOFT_UTILS OFF)
set(ALSOFT_EXAMPLES OFF)
set(ALSOFT_INSTALL OFF)
set(ALSOFT_INSTALL_CONFIG OFF)
set(ALSOFT_INSTALL_HRTF_DATA OFF)
set(ALSOFT_INSTALL_AMBDEC_PRESETS OFF)
set(ALSOFT_INSTALL_EXAMPLES OFF)
set(ALSOFT_INSTALL_UTILS OFF)
set(ALSOFT_UPDATE_BUILD_VERSION OFF)
set(LIBTYPE STATIC)  # To make static library

set(wxBUILD_SHARED OFF)
set(wxUSE_GUI ON)
set(wxBUILD_MONOLITHIC OFF)

project("tiny-music-player"
    VERSION 0.1.0
    LANGUAGES C CXX
)

set(HEADERS
    "${CMAKE_SOURCE_DIR}/src/audio/openal/buffer.h"
    "${CMAKE_SOURCE_DIR}/src/audio/openal/info_and_debug.h"
    "${CMAKE_SOURCE_DIR}/src/audio/openal/listener.h"
    "${CMAKE_SOURCE_DIR}/src/audio/openal/source.h"
    "${CMAKE_SOURCE_DIR}/src/audio/context.h"
    "${CMAKE_SOURCE_DIR}/src/audio/sound_data.h"
    "${CMAKE_SOURCE_DIR}/src/other/vec3.h"
    "${CMAKE_SOURCE_DIR}/src/application.h"
    "${CMAKE_SOURCE_DIR}/src/main_window.h"
)

add_executable(${EXECUTABLE}
    "${CMAKE_SOURCE_DIR}/src/audio/openal/buffer.cpp"
    "${CMAKE_SOURCE_DIR}/src/audio/openal/info_and_debug.cpp"
    "${CMAKE_SOURCE_DIR}/src/audio/openal/listener.cpp"
    "${CMAKE_SOURCE_DIR}/src/audio/openal/source.cpp"
    "${CMAKE_SOURCE_DIR}/src/audio/context.cpp"
    "${CMAKE_SOURCE_DIR}/src/audio/sound_data.cpp"
    "${CMAKE_SOURCE_DIR}/src/application.cpp"
    "${CMAKE_SOURCE_DIR}/src/main_window.cpp"
)

add_library(stb_vorbis STATIC
    "${CMAKE_SOURCE_DIR}/dependencies/stb_vorbis/stb_vorbis.c"
)

target_compile_options(${EXECUTABLE} PRIVATE -Wall -Wextra)

target_compile_features(${EXECUTABLE} PRIVATE cxx_std_17)
set_target_properties(${EXECUTABLE} PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(${EXECUTABLE} PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
    "${CMAKE_SOURCE_DIR}/dependencies/stb_vorbis"
    "${CMAKE_SOURCE_DIR}/extern/spdlog/include"
)

add_subdirectory(extern/openal_soft)
add_subdirectory(extern/wxWidgets)
add_subdirectory(extern/spdlog)

target_link_libraries(${EXECUTABLE} PRIVATE
    OpenAL::OpenAL
    wx::core
    wx::base
    stb_vorbis
    spdlog
)

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    message("Building tiny-music-player in RELEASE mode")

    target_compile_definitions(${EXECUTABLE} PRIVATE
        NDEBUG
    )
endif()
