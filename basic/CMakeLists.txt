cmake_minimum_required(VERSION 3.10)
project(GSTREAMER_TUTORIAL)
find_package(PkgConfig REQUIRED)
find_package(PkgConfig)
#pkg_search_module(GLIB REQUIRED glib-2.0)
pkg_check_modules(GTK3 REQUIRED gtk+-3.0)
pkg_check_modules(GSTREAMER REQUIRED gstreamer-1.0)
#pkg_check_modules(GST_APP REQUIRED gstreamer-app-1.0)
pkg_check_modules(GST_VIDEO REQUIRED gstreamer-video-1.0)
include_directories(
        ${GTK3_INCLUDE_DIRS}
        ${GSTREAMER_INCLUDE_DIRS}
        ${GST_VIDEO_INCLUDE_DIRS}
#        ${GSTREAMER-APP_INCLUDE_DIRS}
#        ${GSTREAMER-AUDIO_INCLUDE_DIRS}
#        ${GSTREAMER-PBUTILS_INCLUDE_DIRS}
#        ${GSTREAMER-FFT_INCLUDE_DIRS}
#        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_executable(basic-tutorial-1 basic-tutorial-1.c)
target_link_libraries(basic-tutorial-1 ${GSTREAMER_LIBRARIES})

add_executable(basic-tutorial-2 basic-tutorial-2.c)
target_link_libraries(basic-tutorial-2 ${GSTREAMER_LIBRARIES})

add_executable(basic-tutorial-3 basic-tutorial-3.c)
target_link_libraries(basic-tutorial-3 ${GSTREAMER_LIBRARIES})

add_executable(basic-tutorial-3_plus basic-tutorial-3_plus.c)
target_link_libraries(basic-tutorial-3_plus ${GSTREAMER_LIBRARIES})

add_executable(basic-tutorial-4 basic-tutorial-4.c)
target_link_libraries(basic-tutorial-4 ${GSTREAMER_LIBRARIES})

add_executable(basic-tutorial-5 basic-tutorial-5.c)
target_link_libraries(basic-tutorial-5 ${GSTREAMER_LIBRARIES} ${GTK3_LIBRARIES} ${GST_VIDEO_LIBRARIES})

add_executable(basic-tutorial-6 basic-tutorial-6.c)
target_link_libraries(basic-tutorial-6 ${GSTREAMER_LIBRARIES})

add_executable(basic-tutorial-6_plus basic-tutorial-6_plus.c)
target_link_libraries(basic-tutorial-6_plus ${GSTREAMER_LIBRARIES})