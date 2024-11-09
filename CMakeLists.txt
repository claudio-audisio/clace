# CMakeList.txt : CMake project for clace, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

message(STATUS "CMake version: ${CMAKE_VERSION}")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O0")

set(CMAKE_CXX_FLAGS_DEBUG "-O0")
set(CMAKE_CXX_FLAGS_RELEASE "-O3")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -fno-omit-frame-pointer")

project ("clace")

# Add source to this project's executable.
add_executable (clace 
	"clace.cpp" "clace.h"
	"ui/userInterface.h" 
	"board/piece.h" "board/board.h" "board/board.cpp" "board/checkStatus.h" "board/checkStatus.cpp" 
	"engine/iengine.h" 
	"game/game.h" "game/game.cpp" "game/player.cpp" "game/player.h" 
	"move/move.h" "move/rollback.h"  "move/rollback.cpp" 
	"common/constants.h" "common/types.h"
	"utils/fen.h" "utils/positions.h" "utils/pieceHelper.h"
  	"utils/boardUtils.h" "engine/r_engine.h" "engine/r_engine.cpp" "utils/utils.h"
	"game/statistics.h" "game/statistics.cpp" "move/movesGenerator.h" "perft/perft.h" "perft/result.h" "perft/perft.cpp" "perft/result.cpp"
	"move/movesCache.h" "common/bitwise.h"
	utils/boardUtils.cpp)
add_library(claceLib 
	"clace.cpp" "clace.h"
	"ui/userInterface.h" 
	"board/piece.h" "board/board.h" "board/board.cpp" "board/checkStatus.h" "board/checkStatus.cpp"  
	"engine/iengine.h" 
	"game/game.h" "game/game.cpp" "game/player.cpp" "game/player.h" 
	"move/move.h" "move/rollback.h"  "move/rollback.cpp" 
	"common/constants.h" "common/types.h"
	"utils/fen.h" "utils/positions.h" "utils/pieceHelper.h"
  	"utils/boardUtils.h" "engine/r_engine.h" "engine/r_engine.cpp" "utils/utils.h"
	"game/statistics.h" "game/statistics.cpp" "move/movesGenerator.h" "perft/perft.h" "perft/result.h" "perft/perft.cpp" "perft/result.cpp"
	"move/movesCache.h" "common/bitwise.h"
	utils/boardUtils.cpp)

if(CMAKE_CXX_STANDARD)
    message(STATUS "C++ standard version is: C++${CMAKE_CXX_STANDARD}")
else()
    message(STATUS "C++ standard version is not set explicitly.")
endif()


############ TESTS ###################################################################################################################################################

include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

enable_testing()

add_executable(
	bitwiseTest
	"tests/bitwiseTest.cpp"
)

add_executable(
  boardTest
  "tests/boardTest.cpp"
)

add_executable(
  boardUtilsTest
  "tests/boardUtilsTest.cpp"
)

add_executable(
  boardPerformanceTest
  "tests/boardPerformanceTest.cpp"
)

add_executable(
  gameTest
  "tests/gameTest.cpp"
)

add_executable(
  fenTest
  "tests/fenTest.cpp"
)

add_executable(
  rollbackTest
  "tests/rollbackTest.cpp"
)

add_executable(
  pieceHelperTest
  "tests/pieceHelperTest.cpp"
)

add_executable(
  moveTest
  "tests/moveTest.cpp"
)

add_executable(
  playerTest
  "tests/playerTest.cpp"
)

add_executable(
  movesGeneratorTest
  "tests/movesGeneratorTest.cpp"
)

add_executable(
  positionsTest
  "tests/positionsTest.cpp"
)

add_executable(
  perftTest
  "tests/perftTest.cpp"
)

add_executable(
	movesCacheTest
	"tests/movesCacheTest.cpp"
)

target_link_libraries(
	bitwiseTest
	GTest::gtest_main
	claceLib
)

target_link_libraries(
  boardTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  boardUtilsTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  boardPerformanceTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  gameTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  fenTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  rollbackTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  pieceHelperTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  moveTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  playerTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  movesGeneratorTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  positionsTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  perftTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
	movesCacheTest
	GTest::gtest_main
	claceLib
)

include(GoogleTest)
gtest_discover_tests(bitwiseTest)
gtest_discover_tests(boardTest)
gtest_discover_tests(boardUtilsTest)
gtest_discover_tests(boardPerformanceTest)
gtest_discover_tests(gameTest)
gtest_discover_tests(fenTest)
gtest_discover_tests(rollbackTest)
gtest_discover_tests(pieceHelperTest)
gtest_discover_tests(moveTest)
gtest_discover_tests(playerTest)
gtest_discover_tests(movesGeneratorTest)
gtest_discover_tests(positionsTest)
gtest_discover_tests(perftTest)
gtest_discover_tests(movesCacheTest)