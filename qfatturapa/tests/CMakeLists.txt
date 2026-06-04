find_package(Qt6 REQUIRED COMPONENTS Core Test)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

include_directories(${Procurans_SOURCE_DIR})
link_directories(${Procurans_BINARY_DIR}/lib)

set(qfatturapa_test_sources
	test_invoiceparser.cpp)

foreach(t IN LISTS qfatturapa_test_sources)
	get_filename_component(name ${t} NAME_WE)
	add_executable(${name} ${t})
	target_link_libraries(${name}
		qfatturapa
		Qt6::Core
		Qt6::Test)
	target_compile_features(${name} PUBLIC cxx_std_17)
	add_test(NAME ${name} COMMAND ${name})
endforeach()
