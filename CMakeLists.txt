cmake_minimum_required(VERSION 3.5.0 FATAL_ERROR)

project(RARStreamer)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)

if(WIN32)
  set(CMAKE_WIN32_EXECUTABLE ON)
  add_definitions(-DOS_WINDOWS)
  set(OS_SPECIFICS resources.rc)
endif()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/CMake/")

find_package(Qt5Widgets REQUIRED)
find_package(UnRAR REQUIRED)

include_directories("${UnRAR_INCLUDE_DIR}")

qt5_add_resources(QRCS resources.qrc)

add_executable(RARStreamer
    src/main.cpp
    src/MainWindow.h
    src/MainWindow.cpp
    src/MainWindow.ui
    src/PasswordDialog.h
    src/PasswordDialog.cpp
    src/PasswordDialog.ui
    src/WorkerThread.h
    src/WorkerThread.cpp
    src/ProgressTracker.h
    src/ProgressTracker.cpp
    src/UnRarHelpers.h
    ${QRCS}
    ${OS_SPECIFICS}
)

target_link_libraries(RARStreamer Qt5::Widgets "${UnRAR_LIBRARY}")

install(TARGETS RARStreamer DESTINATION bin)
install(FILES ${UnRAR_RUNTIME_LIBRARY} DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/USED_LICENSES.txt DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION bin)
