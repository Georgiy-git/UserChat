cmake_minimum_required(VERSION 3.29)

project(project)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

add_executable(User start.cpp
    authorization.hpp authorization.cpp
    window.hpp window.cpp window.ui
)

include_directories(${Boost_INCLUDE_DIRS})

find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

target_sources(User PUBLIC error_code.cpp client.cpp client.hpp)

target_link_libraries(User Qt::Core Qt::Widgets Qt::Gui ws2_32)
