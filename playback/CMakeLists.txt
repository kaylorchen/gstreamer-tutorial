cmake_minimum_required(VERSION 3.10)
project(GSTREAMER_TUTORIAL)
find_package(PkgConfig REQUIRED)
find_package(PkgConfig)
#pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
#pkg_check_modules(GST_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
pkg_check_modules(GST_AUDIO REQUIRED gstreamer-audio-1.0)
pkg_check_modules(GST_pbutils REQUIRED gstreamer-pbutils-1.0)
include_directories(
        ${GTK3_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
        ${GST_VIDEO_INCLUDE_DIRS}
        ${GST_AUDIO_INCLUDE_DIRS}
        ${GST_pbutils_INCLUDE_DIRS}
#        ${GSTREAMER-APP_INCLUDE_DIRS}
#        ${GSTREAMER-AUDIO_INCLUDE_DIRS}
#        ${GSTREAMER-PBUTILS_INCLUDE_DIRS}
#        ${GSTREAMER-FFT_INCLUDE_DIRS}
#        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(playback-tutorial-1 playback-tutorial-1.c)
target_link_libraries(playback-tutorial-1 ${GSTREAMER_LIBRARIES})

add_executable(playback-tutorial-2 playback-tutorial-2.c)
target_link_libraries(playback-tutorial-2 ${GSTREAMER_LIBRARIES})


