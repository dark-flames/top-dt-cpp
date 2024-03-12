include_directories(../include)
add_executable(bin main.cpp)

foreach (module ${module_list})
    message("Link Module: ${module}")
    target_link_libraries(bin PUBLIC ${module})
endforeach ()

set_property(TARGET bin PROPERTY CXX_STANDARD 20)
set_property(TARGET bin PROPERTY CXX_STANDARD_REQUIRED On)