cmake_minimum_required(VERSION 3.1)
project(cmakeqt LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -flto -fomit-frame-pointer -O3 -march=native -s -Wextra -Wall") #-g

find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)

file(GLOB src ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB ui ${CMAKE_SOURCE_DIR}/src/*.ui)
add_executable(${PROJECT_NAME} ${src} ${ui})
qt5_wrap_ui(ui_mainwindow.h ${ui})

target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network)
