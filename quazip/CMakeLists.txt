message(STATUS ${CMAKE_CURRENT_LIST_FILE})

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(${Procurans_SOURCE_DIR})
link_directories(${Procurans_BINARY_DIR}/lib)

find_package(Qt6 REQUIRED COMPONENTS Core)

# We deliberately do NOT set QUAZIP_CAN_USE_QTEXTCODEC so the upstream
# QStringConverter path in quazip_textcodec.h is taken. That keeps us off
# the Qt6Core5Compat module entirely.
configure_file(quazip_config.h.in quazip_config.h @ONLY)

set(quazip_sources
	JlCompress.cpp
	JlCompress.h
	JlOptions.cpp
	QuaCompress.cpp
	QuaCompress.h
	QuaExtract.cpp
	QuaExtract.h
	ioapi.h
	minizip_crypt.h
	qioapi.cpp
	quaadler32.cpp
	quaadler32.h
	quachecksum32.cpp
	quachecksum32.h
	quacrc32.cpp
	quacrc32.h
	quagzipfile.cpp
	quagzipfile.h
	quaziodevice.cpp
	quaziodevice.h
	quazip.cpp
	quazip.h
	quazip_global.h
	quazip_qt_compat.h
	quazip_textcodec.cpp
	quazip_textcodec.h
	quazipdir.cpp
	quazipdir.h
	quazipfile.cpp
	quazipfile.h
	quazipfileinfo.cpp
	quazipfileinfo.h
	quazipnewinfo.cpp
	quazipnewinfo.h
	unzip.c
	unzip.h
	zip.c
	zip.h
)

if (DEFINED QUAZIP_STATIC)
	add_library(quazip STATIC ${quazip_sources})
	target_compile_definitions(quazip PUBLIC QUAZIP_STATIC)
else (DEFINED QUAZIP_STATIC)
	add_library(quazip SHARED ${quazip_sources})
	target_compile_definitions(quazip PRIVATE QUAZIP_BUILD)
endif (DEFINED QUAZIP_STATIC)

set_target_properties(quazip PROPERTIES DEBUG_POSTFIX "d")
target_compile_features(quazip PUBLIC cxx_std_17)
target_include_directories(quazip PUBLIC ${CMAKE_CURRENT_BINARY_DIR})

get_property(zlib_libs GLOBAL PROPERTY zlib_libs_property)
target_link_libraries(quazip Qt6::Core ${zlib_libs})
