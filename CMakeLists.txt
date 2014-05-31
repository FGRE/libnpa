cmake_minimum_required(VERSION 2.8)

# add modules
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# set version
set(LIBNPA_VERSION_MAJOR "0")
set(LIBNPA_VERSION_MINOR "3")
set(LIBNPA_VERSION_PATCH "4")
set(LIBNPA_VERSION "${LIBNPA_VERSION_MAJOR}.${LIBNPA_VERSION_MINOR}.${LIBNPA_VERSION_PATCH}")

# append git revision if available
include(get-git-revision)
get_git_revision(LIBNPA_VERSION_GIT)
if(NOT "${LIBNPA_VERSION_GIT}" STREQUAL "")
    set(LIBNPA_VERSION "${LIBNPA_VERSION}-${LIBNPA_VERSION_GIT}")
endif()

message(STATUS "Configuring libnpa version " ${LIBNPA_VERSION})

# project name and language
project (libnpa CXX)

set(CMAKE_BUILD_TYPE Debug)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# include version number in header
configure_file(${PROJECT_SOURCE_DIR}/include/npaversion.hpp.in
    ${PROJECT_SOURCE_DIR}/include/npaversion.hpp)

find_package(Boost REQUIRED COMPONENTS locale filesystem)

include_directories(${PROJECT_SOURCE_DIR}/include)
add_library(npa SHARED
    src/inpafile.cpp
    src/npafile.cpp
    src/nsbmagic.cpp
    src/onpafile.cpp
    src/scriptfile.cpp
    src/exefile.cpp
    src/fscommon.cpp
)
target_link_libraries(npa ${Boost_LIBRARIES})

# install headers and library
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include/libnpa
    FILES_MATCHING PATTERN "*.hpp")
install(TARGETS npa DESTINATION lib)

# create packages
set(CPACK_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION ${LIBNPA_VERSION})
include(CPack)
