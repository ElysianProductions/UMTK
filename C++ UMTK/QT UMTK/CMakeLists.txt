cmake_minimum_required(VERSION 3.2v)

project(UMTK-Classic LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(Qt6_USE_STATIC_LIBS ON) # Uncomment for static, comment out for dynamic
#set(Qt6_USE_STATIC_RUNTIME ON) #Uncomment for static, comment out for dynamic

find_package(Qt6 COMPONENTS Gui Core Widgets PrintSupport Sql REQUIRED)

# set(INSTALL_DIRECTORY "C:\\BuildDir\\UMTK") # uncomment for static with build directory, comment out for dynamic
set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        mainwidget.cpp
        mainwidget.h
        domainwidget.cpp
        domainwidget.h
        localwidget.cpp
        localwidget.h
        editwidget.cpp
        editwidget.h
        newuser.cpp
        newuser.h
        psintegration.cpp
        psintegration.h
        disablewidget.cpp
        disablewidget.h
        configurationwidget.cpp
        configurationwidget.h
        Images.qrc

)

if(Qt6_FOUND)
    qt_add_executable(UMTK-Classic WIN32
        ${PROJECT_SOURCES}
    )
elseif(NOT Qt6_FOUND)
    add_executable(UMTK-Classic WIN32
        ${PROJECT_SOURCES}
    )
endif()


target_link_libraries(UMTK-Classic PRIVATE Qt::Gui Qt6::Core Qt6::Widgets Qt6::PrintSupport Qt6::Sql ${ADDITIONAL_LIBRARIES}) # Comment out for static, or uncomment for dynamic
#target_link_libraries(UMTK PRIVATE Qt::Gui Qt6::Core Qt6::Widgets Qt6::PrintSupport ${ADDITIONAL_LIBRARIES} -static) # Uncomment for static, comment out for dynamic
