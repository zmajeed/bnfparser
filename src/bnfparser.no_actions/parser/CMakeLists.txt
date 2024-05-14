# bnfparser.no_actions/parser/CMakeLists.txt

project(bnfparser_no_actions_bison)

# standalone parser
add_executable(bnfparse ${BNFPARSER_BISON_CPP_FILE})
target_compile_definitions(bnfparse PRIVATE BUILD_MAIN)
target_link_libraries(bnfparse ${FLEXBISONLIB})

if(CYGWIN)
  target_compile_definitions(bnfparse PRIVATE GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  target_compile_options(bnfparse PRIVATE -Wall -Werror -Wextra -O0 -ggdb -std=c++23 -pthread)
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
# ranges library cannot take -Wall -WX
  target_compile_options(bnfparse PRIVATE -Od)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  target_compile_definitions(bnfparse PRIVATE _SILENCE_CLANG_CONCEPTS_MESSAGE)
endif()

# tests
set(TESTNAME bnfparser.bison.gtest)

add_executable(${TESTNAME} bnfparser.bison.gtest.cpp)

if(CYGWIN)
  target_compile_definitions(${TESTNAME} PRIVATE GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  target_compile_options(${TESTNAME} PRIVATE -Wall -Werror -Wextra -O0 -ggdb -std=c++23 -pthread)
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
# ranges library cannot take -Wall -WX
  target_compile_options(${TESTNAME} PRIVATE -Od)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  target_compile_definitions(${TESTNAME} PRIVATE _SILENCE_CLANG_CONCEPTS_MESSAGE)
endif()

target_link_libraries(${TESTNAME} ${FLEXBISONLIB} gmock_main)

enable_testing()
include(GoogleTest)

