message(STATUS ${CMAKE_CURRENT_LIST_FILE})

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif (NOT MSVC)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

include_directories(${Procurans_SOURCE_DIR})
link_directories(${Procurans_BINARY_DIR})
find_package(Qt5Core REQUIRED)

set(lib_files
		CellAbstract.cpp
		CellAbstract.h
		CellCurrency.cpp
		CellCurrency.h
		CellDate.cpp
		CellDate.h
		CellEmpty.cpp
		CellEmpty.h
		CellFloat.cpp
		CellFloat.h
		CellString.cpp
		CellString.h
		CMakeLists.txt
		Column.cpp
		Column.h
		Document.cpp
		Document.h
		FileAbstract.cpp
		FileAbstract.h
		FileContent.cpp
		FileContent.h
		functions.cpp
		functions.h
		RepeatableAbstract.cpp
		RepeatableAbstract.h
		Row.cpp
		Row.h
		SerializableAbstract.cpp
		SerializableAbstract.h
		StyleableAbstract.cpp
		StyleableAbstract.h
		Table.cpp
		Table.h
)

foreach(f IN LISTS lib_files)
	get_filename_component(b ${f} NAME)
	set_source_files_properties(${f} PROPERTIES
		COMPILE_DEFINITIONS "SRC_FILE_NAME=\"${b}\"")
endforeach()

if (DEFINED ODS_STATIC)
	set(lib_type STATIC)
else (DEFINED ODS_STATIC)
	set(lib_type SHARED)
	add_definitions(-DODS_SHARED -DODS_BUILDING)
endif (DEFINED ODS_STATIC)

add_library(ods ${lib_type} ${lib_files})
target_link_libraries(ods quazip Qt5::Core)