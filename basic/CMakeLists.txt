cmake_minimum_required(VERSION 3.10)
project(GSTREAMER_TUTORIAL)
find_package(PkgConfig REQUIRED)
find_package(PkgConfig)
#pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
#pkg_check_modules(GST_APP REQUIRED gstreamer-app-1.0)
#pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
include_directories(
        ${GSTREAMER_INCLUDE_DIRS}
#        ${GSTREAMER-APP_INCLUDE_DIRS}
#        ${GSTREAMER-AUDIO_INCLUDE_DIRS}
#        ${GSTREAMER-PBUTILS_INCLUDE_DIRS}
#        ${GSTREAMER-FFT_INCLUDE_DIRS}
#        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(hello_world basic-tutorial-1.c)
target_link_libraries(hello_world ${GSTREAMER_LIBRARIES})
