# ebnfparser.no_actions/grammar/CMakeLists.txt

project(ebnfparser_no_actions_grammar)

find_package(FLEX)
find_package(BISON)


# generated source filename should match .l filename
flex_target(flex_files ebnfparser.flex.l ${CMAKE_CURRENT_BINARY_DIR}/ebnfparser.flex.cpp COMPILE_FLAGS -f)

# generated source filename should match .y filename
bison_target(bison_files ebnfparser.bison.y ${CMAKE_CURRENT_BINARY_DIR}/ebnfparser.bison.cpp COMPILE_FLAGS "-Wall -Wdangling-alias -Werror --report lookaheads --report-file bisonreport.txt")

add_flex_bison_dependency(flex_files bison_files)

# export these variables to build standalone parser and lexer in separate directories
set(EBNFPARSER_BISON_CPP_FILE ${BISON_bison_files_OUTPUT_SOURCE} CACHE STRING "" FORCE)
set(EBNFPARSER_FLEX_CPP_FILE ${FLEX_flex_files_OUTPUTS} CACHE STRING "" FORCE)

set(FLEXBISONLIB flexbisonlib.stateful_lexer.actions CACHE STRING "" FORCE)

add_library(${FLEXBISONLIB} STATIC ${FLEX_flex_files_OUTPUTS} ${BISON_bison_files_OUTPUT_SOURCE})
target_compile_definitions(${FLEXBISONLIB} PRIVATE _POSIX_C_SOURCE=200809L)
target_compile_options(${FLEXBISONLIB} PRIVATE -Wall -Werror -Wextra -O0 -ggdb -std=c++23 -pthread)
target_include_directories(${FLEXBISONLIB} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/.. ${CMAKE_CURRENT_BINARY_DIR})
target_link_libraries(${FLEXBISONLIB})
