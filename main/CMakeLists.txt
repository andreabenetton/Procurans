set(COMPANY_NAME "BlueTeam")
set(PRODUCT_NAME "Procurans")
set(COMMENTS "An utility to import Italian electronic bill into odf files")
set(LEGAL_COPYRIGHT "Copyright (c) 2019 BlueTeam")
set(LEGAL_TRADEMARK1 "GNU Affero General Public License ver. 3")
set(LEGAL_TRADEMARK2 "https://www.gnu.org/licenses/agpl-3.0.en.html")
set(MODULE_DESCRIPTION "Procurans")
set(VERSION_MAJOR "0")
set(VERSION_MINOR "1")
set(VERSION_PATCH "1")
set(VERSION_REVISION "0")
	
set(APP_VERSION "${VERSION_MAJOR}.${VERSION_MINOR}.${VERSION_PATCH}.${VERSION_REVISION}")
set(APP_VERSION_RC "${VERSION_MAJOR},${VERSION_MINOR},${VERSION_PATCH},${VERSION_REVISION}")

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

include_directories(${Procurans_SOURCE_DIR})
link_directories(${Procurans_BINARY_DIR}/lib)

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

add_compile_definitions(APP_VERSION="${APP_VERSION}")
add_compile_definitions(COMPANY_NAME="${COMPANY_NAME}")
add_compile_definitions(PRODUCT_NAME="${PRODUCT_NAME}")

if (WIN32)
	configure_file(fileinfo.rc.in fileinfo.rc)

    add_executable(${exe_name} WIN32
        ${src_files}
		${CMAKE_CURRENT_BINARY_DIR}/fileinfo.rc
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



