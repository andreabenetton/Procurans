cmake_minimum_required(VERSION 3.21)
Project(Procurans)

message("Source dir: " ${Procurans_SOURCE_DIR})

set(CMAKE_DEBUG_POSTFIX d)

# Qt 6 requires C++17, and MSVC needs both /std:c++17 and /Zc:__cplusplus —
# without the latter, MSVC defines __cplusplus as 199711L regardless of the
# language standard, and Qt's CMake config rejects the build with a "Qt
# requires a C++17 compiler" error. Set the floor globally so every target
# inherits it before find_package(Qt6) runs.
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)
if(MSVC)
    add_compile_options(/Zc:__cplusplus /permissive-)
endif()

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

if(WIN32 AND NOT CMAKE_PREFIX_PATH)
    # Reasonable default for a fresh dev box; override via
    # -DCMAKE_PREFIX_PATH=... in CMakeSettings.json's cmakeCommandArgs (or
    # an environment variable) when Qt is installed elsewhere or at a
    # different point release.
    set (CMAKE_PREFIX_PATH "C:\\Qt\\6.11.1\\msvc2022_64\\")
endif()

set(LIBRARY_OUTPUT_PATH "${Procurans_BINARY_DIR}/lib")

set (BUILD_STATIC_LIBRARIES "1")
if (BUILD_STATIC_LIBRARIES)
	set(QOASIS_STATIC "1")
	set(QUAZIP_STATIC "1")
	set(ZLIB_STATIC "1")
endif (BUILD_STATIC_LIBRARIES)

if (WIN32)
	add_subdirectory(zlib)
endif(WIN32)

add_subdirectory(quazip)
add_subdirectory(qoasis)
add_subdirectory(qfatturapa)
add_subdirectory(main)

include(CTest)
if (BUILD_TESTING)
	add_subdirectory(qoasis/tests)
	add_subdirectory(qfatturapa/tests)
endif()

# Windows redistributable zip via CPack — `cmake --build build --target package`
# (or `cpack` from the build dir) produces Procurans-<APP_VERSION>-win64.zip
# in the build root. Install rules contributing to the layout live in
# main/CMakeLists.txt; this block only configures the packaging.
if (WIN32)
	set(CPACK_GENERATOR "ZIP")
	set(CPACK_PACKAGE_NAME "Procurans")
	set(CPACK_PACKAGE_VENDOR "BlueTeam")
	set(CPACK_PACKAGE_VERSION "${APP_VERSION}")
	set(CPACK_PACKAGE_FILE_NAME "Procurans-${APP_VERSION}-win64")
	# Don't wrap everything in an extra Procurans-<version>/ directory
	# inside the zip — the customer extracts and runs Procurans.exe
	# directly from the unzip target.
	set(CPACK_INCLUDE_TOPLEVEL_DIRECTORY OFF)
	include(CPack)
endif()
