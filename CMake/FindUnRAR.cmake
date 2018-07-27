# Try to find UnRAR library
# Once done this will define
#  UnRAR_FOUND - if system found UnRAR library
#  UnRAR_INCLUDE_DIRS - The UnRAR include directories
#  UnRAR_LIBRARIES - The libraries needed to use UnRAR

if(NOT UNRAR_ROOT)
  set(UNRAR_ROOT $ENV{UNRAR_ROOT})
endif()

set(UnRAR_ROOT_DIR ${UNRAR_ROOT})

find_path(UnRAR_INCLUDE_DIR
    NAMES unrar.h
    PATHS ${UnRAR_ROOT_DIR}
    DOC "The UnRAR include directory"
)

set(UnRAR_LIBRARY_NAME UnRAR)

if(CMAKE_SIZEOF_VOID_P EQUAL 8)
    # 64 bits
    set(UnRAR_LIBRARY_NAME UnRAR64)
endif()

find_library(UnRAR_LIBRARY 
    NAMES ${UnRAR_LIBRARY_NAME}.lib
    PATHS ${UnRAR_ROOT_DIR}
    PATH_SUFFIXES "x64"
    DOC "The UnRAR library"
)

find_library(UnRAR_RUNTIME_LIBRARY 
    NAMES ${UnRAR_LIBRARY_NAME}.dll
    PATHS ${UnRAR_ROOT_DIR}
    PATH_SUFFIXES "x64"
    DOC "The UnRAR runtime library"
)

include(FindPackageHandleStandardArgs)
# handle the QUIETLY and REQUIRED arguments and set LOGGING_FOUND to TRUE
# if all listed variables are TRUE
find_package_handle_standard_args(UnRAR DEFAULT_MSG UnRAR_INCLUDE_DIR UnRAR_LIBRARY UnRAR_RUNTIME_LIBRARY)

if (UnRAR_FOUND)
    set(UnRAR_LIBRARIES ${UnRAR_LIBRARY} )
    set(UnRAR_RUNTIME_LIBRARIES ${UnRAR_RUNTIME_LIBRARY} )
    set(UnRAR_INCLUDE_DIRS ${UnRAR_INCLUDE_DIR} )
endif()

message("UnRAR Library: ${UnRAR_LIBRARY}")
message("UnRAR Runtime Library: ${UnRAR_RUNTIME_LIBRARY}")

# Tell cmake GUIs to ignore the "local" variables.
mark_as_advanced(UnRAR_ROOT_DIR UnRAR_INCLUDE_DIR UnRAR_LIBRARY UnRAR_RUNTIME_LIBRARY)