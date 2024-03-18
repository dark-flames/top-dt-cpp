macro(add_glob cur_list)
    file(GLOB __tmp RELATIVE ${CMAKE_CURRENT_SOURCE_DIR} CONFIGURE_DEPENDS ${ARGN})
    list(APPEND ${cur_list} ${__tmp})
endmacro()

macro(add_headers_and_sources prefix common_path)
    add_glob(${prefix}_headers ${CMAKE_CURRENT_SOURCE_DIR} ${common_path}/*.h)
    add_glob(${prefix}_sources ${common_path}/*.cpp ${common_path}/*.c ${common_path}/*.h)
endmacro()

set(src_modules)
macro(add_object_library name common_path)
    list(APPEND src_modules ${name})
    set(${name}_sources})
    set(${name}_headers})
    add_headers_and_sources(${name} ${common_path})
    add_library(${name} SHARED ${${name}_sources} ${${name}_headers})
    target_link_libraries(${name} PRIVATE -Wl,--unresolved-symbols=ignore-all)
endmacro()

add_subdirectory(Parser)

add_object_library(common Common)
add_object_library(syntax Syntax)
add_object_library(term Term)
add_object_library(value Value)
add_object_library(declaration Declaration)
add_object_library(normalize Normalize)
add_object_library(pretty Pretty)
add_object_library(exception Exception)
add_object_library(type_check TypeCheck)


set(module_list ${src_modules} PARENT_SCOPE)