find_package(Qt5Test REQUIRED)
find_package(Qt5Core REQUIRED)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(${Procurans_SOURCE_DIR})
include_directories(${Procurans_SOURCE_DIR}/quazip)
link_directories(${Procurans_BINARY_DIR}/lib)

get_property(zlib_libs GLOBAL PROPERTY zlib_libs_property)

set(qoasis_test_sources
	test_currency.cpp
	test_fileods_roundtrip.cpp
	test_repeatvector.cpp
	test_tag.cpp)

foreach(t IN LISTS qoasis_test_sources)
	get_filename_component(name ${t} NAME_WE)
	add_executable(${name} ${t})
	target_link_libraries(${name}
		qoasis
		quazip
		${zlib_libs}
		Qt5::Core
		Qt5::Test)
	target_compile_features(${name} PUBLIC cxx_std_17)
	add_test(NAME ${name} COMMAND ${name})
endforeach()
