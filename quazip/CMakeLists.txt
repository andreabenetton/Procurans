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

# set all include directories for in and out of source builds
include_directories(
	${CMAKE_CURRENT_SOURCE_DIR}
	${CMAKE_CURRENT_BINARY_DIR}
	${ZLIB_INCLUDE_DIRS}
)

file(GLOB_RECURSE src_files
	"*.h" "*.hh" "*.hpp" "*.hxx"
	"*.c" "*.cc" "*.cpp")

if (DEFINED QUAZIP_S)
	set(lib_type STATIC)
	add_compile_definitions(QUAZIP_STATIC)
else ()
	set(lib_type SHARED)
	add_definitions(-DQUAZIP_SHARED -DQUAZIP_BUILD)
endif ()

add_library(quazip ${lib_type} ${src_files})
get_property(zlib_libs GLOBAL PROPERTY zlib_libs_property)
target_link_libraries(quazip Qt5::Core ${zlib_libs})

