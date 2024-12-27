cmake_minimum_required(VERSION 3.16)

project(Toriel-IDE VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets)

set(PROJECT_SOURCES
        src/main.cpp
        src/torielwindow.cpp
        src/torielwindow.h
        torielwindow.ui
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)

qt_add_executable(Toriel-IDE
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        src/theme.h src/theme.cpp
        src/parser.h src/parser.cpp

        src/zenstudio.h src/zenstudio.cpp
    )
else()
    if(ANDROID)
        add_library(Toriel-IDE SHARED
            ${PROJECT_SOURCES}
        )
    else()
        add_executable(Toriel-IDE
            ${PROJECT_SOURCES}
        )
    endif()
endif()

target_link_libraries(Toriel-IDE PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)

add_custom_command(
    TARGET Toriel-IDE POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
        ${CMAKE_SOURCE_DIR}/bin
        ${CMAKE_BINARY_DIR}/bin
)


if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Toriel-IDE)
endif()
set_target_properties(Toriel-IDE PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Toriel-IDE
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Toriel-IDE)
endif()
