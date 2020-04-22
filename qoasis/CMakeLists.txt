message(STATUS ${CMAKE_CURRENT_LIST_FILE})

if (NOT MSVC)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17")
endif (NOT MSVC)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories(${Procurans_SOURCE_DIR})
link_directories(${Procurans_BINARY_DIR})
find_package(Qt5Core REQUIRED)

set(lib_files
    currency.h
    currency.cpp
    fileods.h
    fileods.cpp
    filexml.h
    filexml.cpp
    filecontent.h
    filecontent.cpp
    ifileable.h
    ifileable.cpp
    inameable.h
    inameable.cpp
    irepeatable.h
    irepeatable.cpp
    istyleable.h
    istyleable.cpp
    tag.h
    tag.cpp
    office/body.h
    office/body.cpp
    office/documentcontent.h
    office/documentcontent.cpp
    office/spreadsheet.h
    office/spreadsheet.cpp
    table/tablecell.h
    table/tablecell.cpp
    table/tablecellcurrency.h
    table/tablecellcurrency.cpp
    table/tablecelldate.h
    table/tablecelldate.cpp
    table/tablecellempty.h
    table/tablecellempty.cpp
    table/tablecellfloat.h
    table/tablecellfloat.cpp
    table/tablecellstring.h
    table/tablecellstring.cpp
    table/tablecolumn.h
    table/tablecolumn.cpp
    table/tablerow.h
    table/tablerow.cpp
    table/table.h
    table/table.cpp
)

foreach(f IN LISTS lib_files)
    get_filename_component(b ${f} NAME)
    set_source_files_properties(${f} PROPERTIES COMPILE_DEFINITIONS "SRC_FILE_NAME=\"${b}\"")
endforeach()

if (DEFINED QOASIS_STATIC)
    set(lib_type STATIC)
else (DEFINED QOASIS_STATIC)
    set(lib_type SHARED)
    add_definitions(-DODS_SHARED -DODS_BUILDING)
endif (DEFINED QOASIS_STATIC)

add_library(qoasis ${lib_type} ${lib_files})
target_link_libraries(qoasis quazip Qt5::Core)
