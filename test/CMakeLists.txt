cmake_minimum_required (VERSION 3.7)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the root and call cmake from there")
endif()

project (heapless_test)

#compiler requirement
set (CMAKE_CXX_STANDARD 14)

set(INCLUDE_DIRS 
	"${PROJECT_SOURCE_DIR}/../include/")

include_directories(${INCLUDE_DIRS})

set(SOURCES
	"${PROJECT_SOURCE_DIR}/catch.hpp"
	"${PROJECT_SOURCE_DIR}/main.cpp"
	"${PROJECT_SOURCE_DIR}/test_unordered_map.cpp"
	"${PROJECT_SOURCE_DIR}/test_vector.cpp"
	
	"${INCLUDE_DIRS}/heapless/string.hpp"
	"${INCLUDE_DIRS}/heapless/unordered_map.hpp"
	"${INCLUDE_DIRS}/heapless/vector.hpp"
	
	"${INCLUDE_DIRS}/heapless/impl/basic_small_string.hpp"
	"${INCLUDE_DIRS}/heapless/impl/basic_string.hpp"
	"${INCLUDE_DIRS}/heapless/impl/basic_unordered_map.hpp"
	"${INCLUDE_DIRS}/heapless/impl/basic_vector.hpp"
	"${INCLUDE_DIRS}/heapless/impl/iterator.hpp"
	)

add_executable(heapless_test ${SOURCES})