cmake_minimum_required(VERSION 3.21)

project(UMTKQML VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOMOC ON)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTORCC ON)
#set(APP_ICON_RESOURCE_WINDOWS "${CMAKE_CURRENT_SOURCE_DIR}/Icon.qrc")

find_package(Qt6 6.2 COMPONENTS Quick Core PrintSupport Sql REQUIRED)

qt_add_executable(UMTK-QML WIN32
    main.cpp
    ${APP_ICON_RESOURCE_WINDOWS}
)

qt_add_qml_module(UMTK-QML
    URI UMTKQML
    VERSION 1.0
    QML_FILES main.qml LocalAccountWidget.qml DomainAccountWidget.qml DisableAccountWidget.qml AdvancedSettingsWidget.qml
    SOURCES psintegration.cpp psintegration.h domainintegration.cpp domainintegration.h advancedsettings.cpp advancedsettings.h Icon.qrc
)

target_compile_definitions(UMTK-QML
    PRIVATE $<$<OR:$<CONFIG:Debug>,$<CONFIG:RelWithDebInfo>>:QT_QML_DEBUG>)
target_link_libraries(UMTK-QML
    PRIVATE Qt6::Quick Qt6::PrintSupport Qt6::Sql Qt6::Qml Qt6::Gui Qt6::Core ${ADDITIONAL_LIBRARIES})
