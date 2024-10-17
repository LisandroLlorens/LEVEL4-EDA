cmake_minimum_required(VERSION 3.29)
project(mms_cpp_master)

set(CMAKE_CXX_STANDARD 14)

include_directories(mms-cpp-master)

add_executable(mms_cpp_master
    mms-cpp-master/API.cpp
    mms-cpp-master/API.h
    mms-cpp-master/Main.cpp
        mms-cpp-master/mightyMouseAI.h
        mms-cpp-master/mightyMouseAI.cpp)
