set(HEADER_DIR "${ToyDT_SOURCE_DIR}/include/toy-dt")

add_library(term STATIC term.cpp)

target_include_directories(term PUBLIC ${HEADER_DIR})

set_property(TARGET term PROPERTY CXX_STANDARD 20)
set_property(TARGET term PROPERTY CXX_STANDARD_REQUIRED On)