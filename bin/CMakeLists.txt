include_directories(../include)
add_executable(eval eval_test.cpp)
add_executable(bin main.cpp)

foreach (module ${module_list})
    message("Link Module: ${module}")
    target_link_libraries(eval PUBLIC ${module})
    target_link_libraries(bin PUBLIC ${module})
endforeach ()

target_link_libraries(bin PUBLIC parser)
target_link_libraries(bin PUBLIC parser)

set_property(TARGET bin PROPERTY CXX_STANDARD 20)
set_property(TARGET bin PROPERTY CXX_STANDARD_REQUIRED On)