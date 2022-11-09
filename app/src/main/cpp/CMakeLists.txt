cmake_minimum_required(VERSION 3.18.1)

project("cppcgviewer")

add_library(cppcgviewer SHARED Jni.cpp)

find_library(log-lib log)

target_link_libraries(cppcgviewer ${log-lib})
