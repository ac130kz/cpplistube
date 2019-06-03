cmake_minimum_required(VERSION 3.11 FATAL_ERROR)
project(cpplistube LANGUAGES CXX)

####################################################################################
# building release by default
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE "ReleaseNative")
endif()

# build types
#set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release" "ReleaseNative")

####################################################################################
# qt setup magic
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION TRUE)

####################################################################################
# compiler detection, flag assignment

## gcc/clang (is -stdlib=libc++ better for clang?)
message(STATUS "Using ${CMAKE_CXX_COMPILER_ID}")
if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "Clang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "AppleClang" OR "${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-std=c++17 -Wextra -Wall")
    if ("${CMAKE_BUILD_TYPE}" MATCHES "^Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -flto -fomit-frame-pointer -O3 -s")

        if("${CMAKE_BUILD_TYPE}" STREQUAL "ReleaseNative")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
            message(STATUS "Building ReleaseNative...")
        else()
            message(STATUS "Building Release...")
        endif()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")
        message(STATUS "Building Debug...")
    endif()

## msvc
elseif ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "MSVC")
    set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "/Wall")
    if ("${CMAKE_BUILD_TYPE}" MATCHES "^Release")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Ox /LTCG /GL /Gy /Gw")

        if("${CMAKE_BUILD_TYPE}" STREQUAL "ReleaseNative")
            set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /arch:AVX /arch:AVX2")
            message(STATUS "Building ReleaseNative...")
        else()
            message(STATUS "Building Release...")
        endif()
    else()
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} /Od /DEBUG")
        message(STATUS "Building Debug...")
    endif()
endif()

####################################################################################
# required packages
find_package(Qt5 COMPONENTS Core Widgets Gui Network REQUIRED)

####################################################################################
# setting up source paths
file(GLOB src ${CMAKE_SOURCE_DIR}/src/*.h ${CMAKE_SOURCE_DIR}/src/*.cpp)
file(GLOB ui ${CMAKE_SOURCE_DIR}/src/*.ui)
add_executable(${PROJECT_NAME} ${src} ${ui})
qt5_wrap_ui(ui_mainwindow.h ${ui})

####################################################################################
# linking
target_link_libraries(${PROJECT_NAME} Qt5::Core Qt5::Widgets Qt5::Gui Qt5::Network)
