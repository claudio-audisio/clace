# CMakeList.txt : CMake project for clace, include source and define
# project specific logic here.
#
cmake_minimum_required (VERSION 3.14)

message(STATUS "CMake version: ${CMAKE_VERSION}")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

#add_compile_options("$<$<C_COMPILER_ID:MSVC>:/utf-8>")
#add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")

project ("clace")

# Add source to this project's executable.
add_executable (clace 
	"clace.cpp" "clace.h"
	"io/userInterface.h" 
	"board/piece.h" "board/board.h" "board/board.cpp" "board/checkStatus.h" "board/checkStatus.cpp" 
	"engine/iengine.h" 
	"game/game.h" "game/game.cpp" "game/player.cpp" "game/player.h" 
	"move/move.h" "move/rollback.h" "move/move.cpp" "move/rollback.cpp" 
	"common/constants.h" "common/types.h"
	"utils/fen.h" "utils/castlingInfoHelper.h" "utils/positions.h" "utils/pieceHelper.h"
 )
add_library(claceLib 
	"clace.cpp" "clace.h"
	"io/userInterface.h" 
	"board/piece.h" "board/board.h" "board/board.cpp" "board/checkStatus.h" "board/checkStatus.cpp"  
	"engine/iengine.h" 
	"game/game.h" "game/game.cpp" "game/player.cpp" "game/player.h" 
	"move/move.h" "move/rollback.h" "move/move.cpp" "move/rollback.cpp" 
	"common/constants.h" "common/types.h"
	"utils/fen.h" "utils/castlingInfoHelper.h" "utils/positions.h" "utils/pieceHelper.h"
 )

 if (CMAKE_VERSION VERSION_GREATER 3.12)
  set_property(TARGET clace PROPERTY CXX_STANDARD 20)
endif()

if(CMAKE_CXX_STANDARD)
    message(STATUS "C++ standard version is: C++${CMAKE_CXX_STANDARD}")
else()
    message(STATUS "C++ standard version is not set explicitly.")
endif()

# TODO: Add tests and install targets if needed.
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
  boardTest
  "tests/boardTest.cpp"
)

  add_executable(
  fenTest
  "tests/fenTest.cpp"
 )

target_link_libraries(
  boardTest
  GTest::gtest_main
  claceLib
)

target_link_libraries(
  fenTest
  GTest::gtest_main
  claceLib
)

include(GoogleTest)
gtest_discover_tests(boardTest)
gtest_discover_tests(fenTest)
