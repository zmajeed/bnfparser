# ebnfparser.no_actions/lexer/CMakeLists.txt

project(ebnfparser_no_actions_lexer)

set(TESTNAME ebnfparser_no_actions_lexer.gtest)

add_executable(${TESTNAME} ebnfparser_lexer.gtest.cpp)
# for header file generated by bison
target_include_directories(${TESTNAME} PRIVATE .. ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/..)

if(CYGWIN)
  target_compile_definitions(${TESTNAME} PRIVATE GTEST_HAS_PTHREAD=1 _POSIX_C_SOURCE=200809L)
endif()
if(CMAKE_CXX_COMPILER_ID MATCHES GNU)
  target_compile_options(${TESTNAME} PRIVATE -Wall -Werror -Wextra -O0 -ggdb -std=c++23)
elseif(CMAKE_CXX_COMPILER_ID MATCHES MSVC)
# ranges library cannot take -Wall -WX
  target_compile_options(${TESTNAME} PRIVATE -Od)
elseif(CMAKE_CXX_COMPILER_ID MATCHES Clang)
  target_compile_definitions(${TESTNAME} PRIVATE _SILENCE_CLANG_CONCEPTS_MESSAGE)
endif()

target_link_libraries(${TESTNAME} ${FLEXBISONLIB} gmock_main)

enable_testing()
include(GoogleTest)
gtest_discover_tests(${TESTNAME} EXTRA_ARGS --gtest_color=yes)
