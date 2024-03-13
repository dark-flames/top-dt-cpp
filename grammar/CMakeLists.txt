set(LEXER "${CMAKE_CURRENT_SOURCE_DIR}/ToyLexer.g4")
set(PARSER "${CMAKE_CURRENT_SOURCE_DIR}/ToyParser.g4")

set(LEXER_FILES ToyLexer.cpp ToyLexer.h)
set(PARSER_FILES ToyParser.cpp ToyParser.h)
set(LISTENER_FILES ToyParserBaseListener.cpp ToyParserListener.cpp ToyParserBaseListener.h ToyParserListener.h)

set(FILES ${LEXER_FILES} ${PARSER_FILES} ${LISTENER_FILES})
list(TRANSFORM FILES PREPEND "${CMAKE_CURRENT_SOURCE_DIR}/")

add_custom_command(
        OUTPUT ${FILES}
        COMMAND antlr4 -Dlanguage=Cpp ${LEXER} ${PARSER}
        DEPENDS ${LEXER} ${PARSER}
        COMMENT "Generating lexer and parser"
)

add_library(parser SHARED ${LEXER_FILES} ${PARSER_FILES} ${LISTENER_FILES})
set_target_properties(parser PROPERTIES LINKER_LANGUAGE CXX)
