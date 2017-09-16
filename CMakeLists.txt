cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)
project(qAccordion VERSION 0.2.2)

option(QACCORDION_BUILD_TESTER OFF)
option(QACCORDION_EXTERNAL OFF)

if(WIN32)
    if(${CMAKE_VERSION} VERSION_LESS 3.4.3)
        message(FATAL_ERROR "Windows builds require CMake 3.4.3 or greator")
    else(${CMAKE_VERSION} VERSION_LESS 3.4.3)
        # let cmake handle all the export business on windows
        set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
    endif(${CMAKE_VERSION} VERSION_LESS 3.4.3)
endif(WIN32)

add_subdirectory(demo)
add_subdirectory(include/qaccordion)
add_subdirectory(src)

# use ext_include in parent projects with include directrories. makes sure the
# generated headers are found
if(QACCORDION_EXTERNAL)
    set(EXT_INCLUDE ${EXTERNAL_INCLUDE} PARENT_SCOPE)
endif()
