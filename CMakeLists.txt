cmake_minimum_required(VERSION 3.13.4)
project (kuro)

#include_directories(xml)
file(GLOB SOURCES "*.cpp")
file(GLOB XML_SOURCES "xml/*.cpp")
add_executable(kuro kuro.cpp ${SOURCES} ${XML_SOURCES})
