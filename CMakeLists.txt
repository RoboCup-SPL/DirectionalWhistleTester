cmake_minimum_required(VERSION 3.10)
project(DirectionalWhistleTester)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

find_package(Qt5 COMPONENTS Core Network Widgets REQUIRED)

add_executable(DirectionalWhistleTester
    Src/Challenge.cpp
    Src/ChallengeStartDialog.cpp
    Src/Main.cpp
    Src/MainWindow.cpp
    Src/SPLStandardMessageReceiver.cpp
    Src/TeamList.cpp
)
target_link_libraries(DirectionalWhistleTester Qt5::Core Qt5::Network Qt5::Widgets)
target_include_directories(DirectionalWhistleTester SYSTEM PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/3rdParty/SPL")
