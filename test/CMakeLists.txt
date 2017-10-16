cmake_minimum_required (VERSION 3.2)

if(${CMAKE_SOURCE_DIR} STREQUAL ${CMAKE_BINARY_DIR})
  message(FATAL_ERROR "Prevented in-tree built. Please create a build directory outside of the root and call cmake from there")
endif()

project (fixed_test)

set (CHECK_BOUND FALSE CACHE BOOL "Activate bound checking")

#compiler requirement
if (MSVC)
	add_compile_options(/W4)
	set (CMAKE_CXX_STANDARD 14)
	if (CHECK_BOUND)
		add_compile_options(/DFIXED_CONTAINER_CHECK_BOUND)
	endif()
else ()
	add_compile_options(-Wall -Wextra -Wpedantic -std=c++1z)
	if (CHECK_BOUND)
		add_compile_options(-DFIXED_CONTAINER_CHECK_BOUND)
	endif()
endif()

set(INCLUDE_DIRS 
	"${PROJECT_SOURCE_DIR}/../include/")

set(FIXED_EXPOSED_SOURCE_DIR "${INCLUDE_DIRS}/fixed")
set(FIXED_IMPL_SOURCE_DIR "${FIXED_EXPOSED_SOURCE_DIR}/impl")

include_directories(${INCLUDE_DIRS})

set(SOURCES
	"${PROJECT_SOURCE_DIR}/catch.hpp"
	"${PROJECT_SOURCE_DIR}/main.cpp"
	"${PROJECT_SOURCE_DIR}/test_basic_list_iterators.cpp"
	"${PROJECT_SOURCE_DIR}/test_functions_iterators.hpp"
	"${PROJECT_SOURCE_DIR}/test_functions_unary_containers.hpp"
	"${PROJECT_SOURCE_DIR}/test_list.cpp"
	"${PROJECT_SOURCE_DIR}/test_pointer_iterators.cpp"
	"${PROJECT_SOURCE_DIR}/test_struct.hpp"
	"${PROJECT_SOURCE_DIR}/test_unordered_map.cpp"
	"${PROJECT_SOURCE_DIR}/test_vector.cpp"
		
	"${FIXED_EXPOSED_SOURCE_DIR}/listed_vector.hpp"
	"${FIXED_EXPOSED_SOURCE_DIR}/string.hpp"
	"${FIXED_EXPOSED_SOURCE_DIR}/unordered_map.hpp"
	"${FIXED_EXPOSED_SOURCE_DIR}/vector.hpp"
	
	"${FIXED_IMPL_SOURCE_DIR}/basic_allocation_pattern.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/basic_allocation_sources.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/basic_listed_vector.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/basic_listed_vector_iterator.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/basic_pointer_iterator.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/basic_small_string.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/basic_string.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/basic_unordered_map.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/basic_vector.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/fixed_def.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/fixed_type_traits.hpp"
	"${FIXED_IMPL_SOURCE_DIR}/fixed_type_traits.hpp"
	)

add_executable(fixed_test ${SOURCES})