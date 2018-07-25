cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(RARStreamer)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
endif()

find_package(Qt5Widgets REQUIRED)

add_executable(RARStreamer
    main.cpp
    MainWindow.cpp
    MainWindow.h
    MainWindow.ui
    WorkerThread.cpp
    WorkerThread.h)

target_link_libraries(RARStreamer Qt5::Widgets)
