cmake_minimum_required(VERSION 3.5)
Project(Procurans)

message("Source dir: " ${Procurans_SOURCE_DIR})

#only build zlib on windows because unix has it by default
IF(WIN32)
	include_directories(${Procurans_SOURCE_DIR}/zlib)
	link_directories(${Procurans_BINARY_DIR}/lib)
	set_property(GLOBAL PROPERTY zlib_libs_property "zlib")
ELSE (WIN32)
	find_package(ZLIB REQUIRED)
	if (ZLIB_FOUND)
    	include_directories(${ZLIB_INCLUDE_DIRS})
		set_property(GLOBAL PROPERTY zlib_libs_property "${ZLIB_LIBRARIES}")
	endif(ZLIB_FOUND)
ENDIF (WIN32)

if(WIN32)
    set (CMAKE_PREFIX_PATH "C:\\Qt\\5.14.1\\msvc2017_64\\")
endif()

set(LIBRARY_OUTPUT_PATH "${Procurans_BINARY_DIR}/lib")

set (BUILD_STATIC_LIBRARIES "1")
if (BUILD_STATIC_LIBRARIES)
	set(ODS_STATIC "1")
	set(QUAZIP_S "1")
	set(ZLIB_STATIC "1")
endif (BUILD_STATIC_LIBRARIES)

if (WIN32)
	add_subdirectory(zlib)
endif(WIN32)

add_subdirectory(quazip)
add_subdirectory(ods)
add_subdirectory(main)
