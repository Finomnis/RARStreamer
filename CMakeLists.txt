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

target_link_libraries(RARStreamer Qt5::Widgets "${UnRAR_LIBRARY}")

# Copy UnRAR runtime library to binary path, necessary to make RARStreamer executable from within QtCreator
add_custom_command(TARGET RARStreamer POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy "${UnRAR_RUNTIME_LIBRARY}" "$<TARGET_FILE_DIR:RARStreamer>"
)

install(TARGETS RARStreamer RUNTIME DESTINATION RARStreamer)
