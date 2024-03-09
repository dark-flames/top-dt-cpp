add_executable(bin main.cpp
        ../src/Common/Visitor.cpp
        ../src/Pretty/Term.cpp)

foreach (module ${module_list})
    target_include_directories (${module} ${ARGN} PRIVATE )
    target_link_libraries (${module} ${ARGN} PRIVATE )
endforeach ()

set_property(TARGET bin PROPERTY CXX_STANDARD 20)
set_property(TARGET bin PROPERTY CXX_STANDARD_REQUIRED On)