cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(RARStreamer)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
  add_definitions(-DOS_WINDOWS)
  set(OS_SPECIFICS resources.rc)
endif()

find_package(Qt5Widgets REQUIRED)

include_directories("ext")
link_directories("ext")

qt5_add_resources(QRCS resources.qrc)

add_executable(RARStreamer
    main.cpp
    MainWindow.h
    MainWindow.cpp
    MainWindow.ui
    PasswordDialog.h
    PasswordDialog.cpp
    PasswordDialog.ui
    WorkerThread.h
    WorkerThread.cpp
    ProgressTracker.h
    ProgressTracker.cpp
    UnRarHelpers.h
    ${QRCS}
    ${OS_SPECIFICS}
)

target_link_libraries(RARStreamer Qt5::Widgets UnRAR)
install(TARGETS RARStreamer RUNTIME DESTINATION bin)
