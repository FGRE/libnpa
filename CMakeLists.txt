cmake_minimum_required(VERSION 2.8)
project (libnpa)

find_package(Boost REQUIRED COMPONENTS locale)

set(CMAKE_BUILD_TYPE Debug)
include_directories(${PROJECT_SOURCE_DIR}/include)
add_definitions(-std=c++11)

aux_source_directory(${PROJECT_SOURCE_DIR}/src/ LIBNPA_SRC)

add_library(npa SHARED ${LIBNPA_SRC})
target_link_libraries(npa ${Boost_LIBRARIES})
set_target_properties(npa PROPERTIES LINKER_LANGUAGE CXX)
