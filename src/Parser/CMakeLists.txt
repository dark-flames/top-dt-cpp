FLEX_TARGET(FLexer Lexer.l ${CMAKE_CURRENT_BINARY_DIR}/Lexer.yy.cpp)

BISON_TARGET(BParser Parser.y ${CMAKE_CURRENT_BINARY_DIR}/Parser.tab.cpp
        DEFINES_FILE ${CMAKE_CURRENT_BINARY_DIR}/Parser.tab.h)

ADD_FLEX_BISON_DEPENDENCY(FLexer BParser)

include_directories(${FLEX_INCLUDE_DIRS})

message(${CMAKE_CURRENT_BINARY_DIR})
file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h)
message(${SOURCE_FILES} ${BISON_BParser_OUTPUT_HEADER} ${FLEX_FLexer_OUTPUTS} ${BISON_BParser_OUTPUT_SOURCE})


add_library(parser STATIC ${BISON_BParser_OUTPUTS} ${FLEX_FLexer_OUTPUTS} ${SOURCE_FILES})
target_include_directories(parser BEFORE PUBLIC ${CMAKE_CURRENT_BINARY_DIR})