# CMakeList.txt : CMake project for clace, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

message(STATUS "CMake version: ${CMAKE_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_CXX_FLAGS_DEBUG "-O0 -g")
set(CMAKE_CXX_FLAGS_RELEASE "-O3 -march=native -flto=auto")

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer")

if(CMAKE_CXX_STANDARD)
	message(STATUS "C++ standard version is: C++${CMAKE_CXX_STANDARD}")
else()
	message(STATUS "C++ standard version is not set explicitly.")
endif()

set(RAYLIB_INCLUDE_DIR "/home/claudio/Projects/raylib/src")
set(RAYLIB_LIB_DIR "/home/claudio/Projects/raylib/src")

include_directories(${RAYLIB_INCLUDE_DIR})

project ("clace")

if (CMAKE_BUILD_TYPE STREQUAL "Debug")
	add_compile_definitions(DEBUG_MODE)
endif()

if (CMAKE_BUILD_TYPE STREQUAL "Release")
	add_compile_definitions(RELEASE_MODE)
endif()

find_package(spdlog REQUIRED)

set(SRC_FOLDERS
		"board"
		"cache"
		"common"
		"engine"
		"evaluation"
		"game"
		"move"
		"perft"
		"uci"
		"ui"
		"utils"
)

foreach(dir ${SRC_FOLDERS})
	file(GLOB_RECURSE FOLDER_FILES
			CONFIGURE_DEPENDS
			"${dir}/*.cpp"
			"${dir}/*.h"
	)
	list(APPEND ALL_SOURCES ${FOLDER_FILES})
endforeach()

list(APPEND ALL_SOURCES "clace.h")

add_library(claceLib ${ALL_SOURCES})

target_include_directories(claceLib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

add_executable(clace clace.cpp)

target_link_libraries(claceLib PRIVATE spdlog::spdlog)
target_link_libraries(claceLib PRIVATE ${RAYLIB_LIB_DIR}/libraylib.a -lm -lpthread -ldl -lrt -lX11)

target_link_libraries(clace PRIVATE claceLib)

#[[target_link_libraries(clace
		${RAYLIB_LIB_DIR}/libraylib.a
		-lm -lpthread -ldl -lrt -lX11
)]]


############ TESTS ###################################################################################################################################################
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

include(GoogleTest)
enable_testing()

set(TEST_NAMES
	phaseTest
	bitwiseTest
	boardTest
	boardUtilsTest
	performanceDebugTest
	performanceReleaseTest
	performancePerftTest
	performanceEngineTest
	gameTest
	fenTest
	rollbackTest
	pieceHelperTest
	moveTest
	playerTest
	movesGeneratorTest
	positionsTest
	perftTest
	evaluationTest
	bfEngineTest
	abEngineTest
	rayAttacksTest
	movesCalculationTest
	zobristTest
	transpositionTableTest
	arrayPoolTest
)

foreach(test ${TEST_NAMES})
	add_executable(${test} "tests/${test}.cpp")
	target_link_libraries(${test} GTest::gtest_main claceLib)
	gtest_discover_tests(${test})
endforeach()


