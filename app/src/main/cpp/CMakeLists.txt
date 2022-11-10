cmake_minimum_required(VERSION 3.18.1)

project("cppcgviewer")

add_library(jni SHARED Jni.cpp)
add_library(cppcgviewer SHARED ModelData.cpp)

find_library(log-lib log)

target_link_libraries(jni ${log-lib} android cppcgviewer)
target_link_libraries(cppcgviewer ${log-lib})
