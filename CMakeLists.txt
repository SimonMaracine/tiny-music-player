cmake_minimum_required(VERSION 3.20)

set(CMAKE_POLICY_DEFAULT_CMP0077 NEW)

set(EXECUTABLE_NAME "tiny-music-player")

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
    "${CMAKE_SOURCE_DIR}/src/application.h"
)

add_executable(${EXECUTABLE_NAME}
    "${CMAKE_SOURCE_DIR}/src/main.cpp"
    "${CMAKE_SOURCE_DIR}/src/application.cpp"
)

target_compile_features(${EXECUTABLE_NAME} PRIVATE cxx_std_17)
set_target_properties(${EXECUTABLE_NAME} PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(${EXECUTABLE_NAME} PRIVATE
    "${CMAKE_SOURCE_DIR}/src"
)

add_subdirectory(extern/openal_soft)
add_subdirectory(extern/wxWidgets)

target_link_libraries(${EXECUTABLE_NAME}
    OpenAL::OpenAL
    wx::core
    wx::base
)

if(CMAKE_BUILD_TYPE MATCHES Release)
    message("Building tiny-music-player in RELEASE mode")

    target_compile_definitions(${EXECUTABLE_NAME} PRIVATE
        NDEBUG
    )
endif()
