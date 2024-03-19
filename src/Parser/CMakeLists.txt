add_custom_command(
        OUTPUT Lexer.yy.cpp COMMAND
        flex Lexer.l
)

add_custom_command(
        OUTPUT Parser.tab.h Parser.tab.cpp COMMAND
        bison Parser.y
)

file(GLOB SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.cpp ${CMAKE_CURRENT_SOURCE_DIR}/*.h ${CMAKE_CURRENT_SOURCE_DIR}/*.hh)


add_library(parser STATIC ${SOURCE_FILES})
#target_link_libraries(parser PRIVATE -Wl,--unresolved-symbols=ignore-all)