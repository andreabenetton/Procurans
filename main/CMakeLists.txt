
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${Ods2_SOURCE_DIR})
link_directories(${Ods2_BINARY_DIR}/lib)

if (NOT MSVC)
	set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")
endif (NOT MSVC)

#find_package(Qt5LinguistTools REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets  REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Xml REQUIRED)

set(src_files
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    comboboxitemdelegate.cpp
    comboboxitemdelegate.h
    gridschemafield.h
    gridschemafield.cpp
    odsdocument.h
    odsdocument.cpp
    odsfile.h
    odsfile.cpp
    odscell.h
    odscell.cpp
    settings.cpp
    settings.h
    logger.cpp
    logger.h)

qt5_add_resources(src_files application.qrc)

foreach(f IN LISTS src_files)
	get_filename_component(b ${f} NAME)
	set_source_files_properties(${f} PROPERTIES
		COMPILE_DEFINITIONS "SRC_FILE_NAME=\"${b}\"")
endforeach()

include_directories(${Procurans_SOURCE_DIR}/quazip)

set (exe_name "Procurans")

if (WIN32)
    add_executable(${exe_name} WIN32
        ${src_files}
        #${TS_FILES}
)
else ()
    add_executable(${exe_name}
        ${src_files}
        #${TS_FILES}
)
endif ()

get_property(zlib_libs GLOBAL PROPERTY zlib_libs_property)

target_link_libraries(${exe_name}
    quazip
    ${zlib_libs}
    Qt5::Widgets
    Qt5::XmlPatterns)



