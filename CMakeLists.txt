cmake_minimum_required(VERSION 2.8)

# add modules
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# set version
set(LIBNPA_VERSION_MAJOR "0")
set(LIBNPA_VERSION_MINOR "4")
set(LIBNPA_VERSION_PATCH "0")
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

find_package(Boost REQUIRED COMPONENTS system locale filesystem)
find_package(ZLIB REQUIRED)
find_package(FLEX REQUIRED)
find_package(BISON REQUIRED)

string(REPLACE "." ";" VERSION_LIST ${FLEX_VERSION})
list(GET VERSION_LIST 0 FLEX_VERSION_MAJOR)
list(GET VERSION_LIST 1 FLEX_VERSION_MINOR)
list(GET VERSION_LIST 2 FLEX_VERSION_PATCH)
add_definitions(-DFLEX_VERSION_MAJOR=${FLEX_VERSION_MAJOR})
add_definitions(-DFLEX_VERSION_MINOR=${FLEX_VERSION_MINOR})
add_definitions(-DFLEX_VERSION_PATCH=${FLEX_VERSION_PATCH})

include_directories(${PROJECT_SOURCE_DIR}/include
    ${Boost_INCLUDE_DIR}
    ${ZLIB_INCLUDE_DIR}
)

flex_target(lexer ${CMAKE_SOURCE_DIR}/src/lexer.l ${CMAKE_SOURCE_DIR}/src/lexer.cpp)
bison_target(parser ${CMAKE_SOURCE_DIR}/src/parser.y ${CMAKE_SOURCE_DIR}/src/parser.cpp)

add_library(npa SHARED
    src/inpafile.cpp
    src/npafile.cpp
    src/nsbmagic.cpp
    src/onpafile.cpp
    src/scriptfile.cpp
    src/exefile.cpp
    src/fscommon.cpp
    src/inipafile.cpp
    src/isgfile.cpp
    src/npakeys.cpp
    src/buffer.cpp
    src/nsscompiler.cpp
    src/parser.cpp
    src/lexer.cpp
    src/nsbconstants.cpp
)
target_link_libraries(npa ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})

# install headers and library
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include/libnpa
    FILES_MATCHING PATTERN "*.hpp")
install(TARGETS npa DESTINATION lib)

# create packages
set(CPACK_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION ${LIBNPA_VERSION})
include(CPack)
