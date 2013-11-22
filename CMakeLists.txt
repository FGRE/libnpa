cmake_minimum_required(VERSION 2.8)
project (libnpa)

if (CMAKE_COMPILER_IS_GNUCXX)
    add_definitions(-std=gnu++11)
endif()

find_package(Boost REQUIRED COMPONENTS locale)

set(CMAKE_BUILD_TYPE Debug)
include_directories(${CMAKE_SOURCE_DIR}/include)

file(
GLOB_RECURSE
LIBNPA_SRC
${CMAKE_SOURCE_DIR}/src/*
)

add_library(npa SHARED ${LIBNPA_SRC})
set_target_properties(npa PROPERTIES LINKER_LANGUAGE CXX)
