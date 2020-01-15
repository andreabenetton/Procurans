cmake_minimum_required(VERSION 3.5)

project(Procurans LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# QtCreator supports the following variables for Android, which are identical to qmake Android variables.
# Check http://doc.qt.io/qt-5/deployment-android.html for more information.
# They need to be set before the find_package(Qt5 ...) call.

#if(ANDROID)
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
#    if (ANDROID_ABI STREQUAL "armeabi-v7a")
#        set(ANDROID_EXTRA_LIBS
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libcrypto.so
#            ${CMAKE_CURRENT_SOURCE_DIR}/path/to/libssl.so)
#    endif()
#endif()

find_package(Qt5 COMPONENTS Widgets LinguistTools REQUIRED)
find_package(Qt5Core REQUIRED)
find_package(Qt5Gui REQUIRED)
find_package(Qt5XmlPatterns REQUIRED)
find_package(Qt5Sql REQUIRED)
find_package(Qt5Xml REQUIRED)


set(TS_FILES Procurans_it_IT.ts)

set(SOURCES
    main.cpp
    mainwindow.cpp
    mainwindow.h
    mainwindow.ui
    comboboxitemdelegate.cpp
    comboboxitemdelegate.h
    gridschemaitem.h
    gridschemaitem.cpp
    settings.cpp
    settings.h
    ods/ods)

qt5_add_resources(SOURCES application.qrc)

if(ANDROID)
  add_library(Procurans SHARED
      ${SOURCES}
      ${TS_FILES}
  )
else()
  add_executable(Procurans
      ${SOURCES}
      ${TS_FILES}
  )
endif()


target_link_libraries(Procurans PRIVATE
    Qt5::Widgets
    Qt5::XmlPatterns
    ${CMAKE_SOURCE_DIR}/lib/libods.a
    ${CMAKE_SOURCE_DIR}/lib/libquazip.a
    ${CMAKE_SOURCE_DIR}/lib/libzlib.a)

qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
