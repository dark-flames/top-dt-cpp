set(HEADER_DIR "${ToyDT_SOURCE_DIR}/include/toy-dt")

add_library(pretty STATIC term.cpp)

target_include_directories(pretty PUBLIC ${HEADER_DIR})

set_property(TARGET pretty PROPERTY CXX_STANDARD 20)
set_property(TARGET pretty PROPERTY CXX_STANDARD_REQUIRED On)