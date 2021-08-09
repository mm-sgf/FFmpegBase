cmake_minimum_required(VERSION 3.10.2)

project("ffplay")

file(GLOB cpp_source *.cpp *.h)

include_directories(include)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -L${CMAKE_SOURCE_DIR}/libs/${ANDROID_ABI}")

add_library(
             native-lib
             SHARED
        ${cpp_source})
find_library(
            log-lib
            log)

target_link_libraries(
                       native-lib
                        avcodec
                        avdevice
                        avfilter
                        avformat
                        avutil
                        swresample
                        swscale
                        android
                       ${log-lib} )