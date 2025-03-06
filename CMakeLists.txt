cmake_minimum_required(VERSION 3.29)

project(ChatUser)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(ChatUser start.cpp
    authorization.hpp authorization.cpp
    window.hpp window.cpp window.ui
    images_sources.qrc
    sqlite3.h sqlite3.c
    newchatline.hpp newchatline.cpp
    onlyacceptlineedit.hpp onlyacceptlineedit.cpp
    findfile.hpp findfile.cpp findfile.ui
    finddir.hpp finddir.cpp finddir.ui
)

set_target_properties(ChatUser PROPERTIES WIN32_EXECUTABLE ON)

include_directories(${Boost_INCLUDE_DIRS} ${Qt6Core_INCLUDE_DIRS})

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

target_sources(ChatUser PUBLIC error_code.cpp client.cpp client.hpp)

target_link_libraries(ChatUser Qt6::Core Qt6::Widgets Qt6::Gui ws2_32)
