cmake_minimum_required(VERSION 3.10)
project(GSTREAMER_TUTORIAL)
find_package(PkgConfig REQUIRED)
find_package(PkgConfig)
#pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
pkg_check_modules(GST_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
pkg_check_modules(GST_AUDIO REQUIRED gstreamer-audio-1.0)
pkg_check_modules(GST_pbutils REQUIRED gstreamer-pbutils-1.0)

include_directories(
        ${GTK3_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
        ${GST_VIDEO_INCLUDE_DIRS}
        ${GST_AUDIO_INCLUDE_DIRS}
        ${GST_pbutils_INCLUDE_DIRS}
        ${GST_APP_INCLUDE_DIRS}
#        ${GSTREAMER-AUDIO_INCLUDE_DIRS}
#        ${GSTREAMER-PBUTILS_INCLUDE_DIRS}
#        ${GSTREAMER-FFT_INCLUDE_DIRS}
#        ${CMAKE_CURRENT_SOURCE_DIR}
)
add_executable(appsrc_ex appsrc_ex.c)
target_link_libraries(appsrc_ex ${GSTREAMER_LIBRARIES} ${GST_APP_LIBRARIES})
