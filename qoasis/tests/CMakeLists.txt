find_package(Qt6 REQUIRED COMPONENTS Core Test)

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
	test_tablecell.cpp
	test_tablecolumn.cpp
	test_tag.cpp)

foreach(t IN LISTS qoasis_test_sources)
	get_filename_component(name ${t} NAME_WE)
	add_executable(${name} ${t})
	target_link_libraries(${name}
		qoasis
		quazip
		${zlib_libs}
		Qt6::Core
		Qt6::Test)
	target_compile_features(${name} PUBLIC cxx_std_17)
	add_test(NAME ${name} COMMAND ${name})
endforeach()
