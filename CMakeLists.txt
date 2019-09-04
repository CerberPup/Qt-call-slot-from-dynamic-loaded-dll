cmake_minimum_required(VERSION 3.0.0)
project(App VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(Headers)
include_directories(Libka)
file(GLOB SOURCES
    Headers/*.h
    Sources/*.cpp
)

set(CMAKE_PREFIX_PATH /home/wojtek/Dev/Qt/)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()

find_package(Qt5 COMPONENTS Widgets REQUIRED)
find_package(Qt5 COMPONENTS Core REQUIRED)
find_package(Qt5 COMPONENTS Network REQUIRED)

add_library(pizdryk SHARED Libka/main.cpp Libka/main.h)

add_executable(App ${SOURCES} )

#TARGET_LINK_LIBRARIES(App pizdryk)
target_link_libraries(App Qt5::Widgets Qt5::Core Qt5::Network)
target_link_libraries(App dl) #GLIBC
target_link_libraries(pizdryk Qt5::Widgets Qt5::Core Qt5::Network)

set_target_properties(App     PROPERTIES CXX_STANDARD 20)
set_target_properties(pizdryk PROPERTIES CXX_STANDARD 20)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
