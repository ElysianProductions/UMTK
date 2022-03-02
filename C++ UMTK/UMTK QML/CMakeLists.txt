cmake_minimum_required(VERSION 3.16)

project(UMTKQML VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(Qt6 6.2 COMPONENTS Quick Core PrintSupport REQUIRED)

qt_add_executable(appUMTKQML
    main.cpp
)

qt_add_qml_module(appUMTKQML
    URI UMTKQML
    VERSION 1.0
    QML_FILES main.qml LocalAccountWidget.qml DomainAccountWidget.qml DisableAccountWidget.qml
    SOURCES psintegration.cpp psintegration.h domainintegration.cpp domainintegration.h
)

target_compile_definitions(appUMTKQML
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(appUMTKQML
    PRIVATE Qt6::Quick Qt6::PrintSupport ${ADDITIONAL_LIBRARIES})
