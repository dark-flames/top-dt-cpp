add_executable(bin main.cpp)

target_link_libraries(bin PRIVATE term)
target_link_libraries(bin PRIVATE pretty)

set_property(TARGET bin PROPERTY CXX_STANDARD 20)
set_property(TARGET bin PROPERTY CXX_STANDARD_REQUIRED On)