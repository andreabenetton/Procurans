message(STATUS ${CMAKE_CURRENT_LIST_FILE})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(${Procurans_SOURCE_DIR})
link_directories(${Procurans_BINARY_DIR})
find_package(Qt6 REQUIRED COMPONENTS Core)

set(lib_files
    codelists.h
    codelists.cpp
    invoiceparser.h
    invoiceparser.cpp
)

foreach(f IN LISTS lib_files)
    get_filename_component(b ${f} NAME)
    set_source_files_properties(${f} PROPERTIES COMPILE_DEFINITIONS "SRC_FILE_NAME=\"${b}\"")
endforeach()

if (DEFINED QOASIS_STATIC)
    set(lib_type STATIC)
else (DEFINED QOASIS_STATIC)
    set(lib_type SHARED)
endif (DEFINED QOASIS_STATIC)

add_library(qfatturapa ${lib_type} ${lib_files})
set_target_properties(qfatturapa PROPERTIES DEBUG_POSTFIX "d")
target_compile_features(qfatturapa PUBLIC cxx_std_17)
target_link_libraries(qfatturapa Qt6::Core)
