cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(RARStreamer)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
  add_definitions(-DOS_WINDOWS)
endif()

find_package(Qt5Widgets REQUIRED)

include_directories("ext")
link_directories("ext")

add_executable(RARStreamer
    main.cpp
    MainWindow.h
    MainWindow.cpp
    MainWindow.ui
    WorkerThread.h
    WorkerThread.cpp
    ProgressTracker.h
    ProgressTracker.cpp
    UnRarHelpers.h
)

target_link_libraries(RARStreamer Qt5::Widgets UnRAR)
