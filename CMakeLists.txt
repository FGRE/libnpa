cmake_minimum_required(VERSION 2.8)

# add modules
set (CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules")

# set version
set(LIBNPA_VERSION_MAJOR "0")
set(LIBNPA_VERSION_MINOR "5")
set(LIBNPA_VERSION_PATCH "1")
set(LIBNPA_VERSION "${LIBNPA_VERSION_MAJOR}.${LIBNPA_VERSION_MINOR}.${LIBNPA_VERSION_PATCH}")

# make sure all symbols are exported on Windows
# currently requires cmake-dev (CMake 3.3.20150721 or newer)
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

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

# msvc doesn't have unistd.h, the equivalent is io.h
# thus, a dummy unistd.h that includes io.h is used
# if msvc is detected
if(MSVC)
	include_directories(${PROJECT_SOURCE_DIR}/include/msvc)
endif()

flex_target(lexer ${CMAKE_SOURCE_DIR}/src/lexer.l ${CMAKE_SOURCE_DIR}/src/lexer.cpp)
bison_target(parser ${CMAKE_SOURCE_DIR}/src/parser.y ${CMAKE_SOURCE_DIR}/src/parser.cpp)
add_flex_bison_dependency(lexer parser)

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
    ${BISON_parser_OUTPUTS}
    ${FLEX_lexer_OUTPUTS}
    src/nsbconstants.cpp
)
target_link_libraries(npa ${Boost_LIBRARIES} ${ZLIB_LIBRARIES})

# run tests with custom 'check' target
enable_testing()
add_executable(test_version EXCLUDE_FROM_ALL tests/version.cpp)
target_link_libraries(test_version npa)
add_test(VersionNumbers test_version)
add_executable(test_xor EXCLUDE_FROM_ALL tests/xor.cpp)
target_link_libraries(test_xor npa)
add_test(EncryptDecrypt test_xor)
add_custom_target(check COMMAND ${CMAKE_CTEST_COMMAND})
add_dependencies(check test_version test_xor)

# install headers and library
install(DIRECTORY ${PROJECT_SOURCE_DIR}/include/
    DESTINATION include/libnpa
    FILES_MATCHING PATTERN "*.hpp")
install(TARGETS npa DESTINATION lib)

# create packages
set(CPACK_GENERATOR "TBZ2")
set(CPACK_PACKAGE_VERSION ${LIBNPA_VERSION})
include(CPack)
