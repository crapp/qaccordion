cmake_minimum_required(VERSION 3.6 FATAL_ERROR)
project(qAccordion VERSION 0.2.2)

option(QACCORDION_BUILD_DEMO OFF)
option(QACCORDION_EXTERNAL OFF)

# let cmake handle all the export business on windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(demo)
add_subdirectory(include/qaccordion)
add_subdirectory(src)

# use ext_include in parent projects with include directrories. makes sure the
# generated headers are found
if(QACCORDION_EXTERNAL)
    set(EXT_INCLUDE ${EXTERNAL_INCLUDE} PARENT_SCOPE)
endif()
