cmake_minimum_required(VERSION 3.16)
project(streamViewer VERSION 0.1 LANGUAGES CXX)

# Set CMake policies to control behavior
cmake_policy(SET CMP0071 NEW)
cmake_policy(SET CMP0074 NEW)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Find Qt package
find_package(Qt6 REQUIRED COMPONENTS Widgets Multimedia MultimediaWidgets)

# Find GStreamer packages
find_package(PkgConfig REQUIRED)
pkg_check_modules(GSTREAMER REQUIRED IMPORTED_TARGET gstreamer-1.0)
pkg_check_modules(GSTREAMER_BASE REQUIRED IMPORTED_TARGET gstreamer-base-1.0)
pkg_check_modules(GSTREAMER_VIDEO REQUIRED IMPORTED_TARGET gstreamer-video-1.0)

# Set MinGW specific flags
if(MINGW)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -municode")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} -static-libgcc -static-libstdc++ -Wl,--allow-multiple-definition")
endif()

# Define source files
set(PROJECT_SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
)

# Add executable
qt_add_executable(streamViewer
    MANUAL_FINALIZATION
    ${PROJECT_SOURCES}
    videowidget.h
    videowidget.cpp
    playbackwidget.h
    playbackwidget.cpp
)

# Set target properties
set_target_properties(streamViewer PROPERTIES
    WIN32_EXECUTABLE TRUE
    MACOSX_BUNDLE TRUE
)

# Link libraries
target_link_libraries(streamViewer PRIVATE
    Qt6::Widgets
    Qt6::Multimedia
    Qt6::MultimediaWidgets
    PkgConfig::GSTREAMER
    PkgConfig::GSTREAMER_BASE
    PkgConfig::GSTREAMER_VIDEO
)

# Include directories
target_include_directories(streamViewer PRIVATE
    ${GSTREAMER_INCLUDE_DIRS}
    ${GSTREAMER_BASE_INCLUDE_DIRS}
    ${GSTREAMER_VIDEO_INCLUDE_DIRS}
)

# Add definitions
target_compile_definitions(streamViewer PRIVATE
    ${GSTREAMER_DEFINITIONS}
    ${GSTREAMER_BASE_DEFINITIONS}
    ${GSTREAMER_VIDEO_DEFINITIONS}
)

# Installation
include(GNUInstallDirs)
install(TARGETS streamViewer
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

# Finalize the executable
qt_finalize_executable(streamViewer)
