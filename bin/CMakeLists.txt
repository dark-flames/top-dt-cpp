add_executable(bin main.cpp)

foreach (module ${module_list})
    target_link_libraries(bin PUBLIC ${module})
endforeach ()

set_property(TARGET bin PROPERTY CXX_STANDARD 20)
set_property(TARGET bin PROPERTY CXX_STANDARD_REQUIRED On)