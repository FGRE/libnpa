cmake_minimum_required(VERSION 2.8)
project (libnpa CXX)

find_package(Boost REQUIRED COMPONENTS locale)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(npa SHARED
src/inpafile.cpp
src/npafile.cpp
src/nsbmagic.cpp
src/onpafile.cpp
src/scriptfile.cpp)
target_link_libraries(npa ${Boost_LIBRARIES})

# install headers and library
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/ DESTINATION include/libnpa)
install(TARGETS npa DESTINATION lib)
