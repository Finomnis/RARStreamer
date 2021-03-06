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

add_definitions(-DRARSTREAMER_VERSION="$ENV{APPVEYOR_BUILD_VERSION}")

if (UNIX)
    add_definitions(-D_UNIX)
    set(EXTRA_LINKER_FLAGS "-pthread")
endif (UNIX)

include_directories("${UnRAR_INCLUDE_DIRS}")

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

target_link_libraries(RARStreamer Qt5::Widgets "${UnRAR_LIBRARIES}" "${EXTRA_LINKER_FLAGS}")

install(TARGETS RARStreamer DESTINATION bin)
if(WIN32)
    install(FILES ${UnRAR_RUNTIME_LIBRARIES} DESTINATION bin)
endif(WIN32)
install(FILES ${CMAKE_SOURCE_DIR}/Used_Licenses.txt DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE DESTINATION bin)
install(FILES ${CMAKE_SOURCE_DIR}/LICENSE.rtf DESTINATION bin)
