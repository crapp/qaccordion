CMAKE_MINIMUM_REQUIRED(VERSION 3.0)
PROJECT(qAccordion)

option (QACCORDION_BUILD_TESTER OFF)
option (QACCORDION_EXTERNAL OFF)

set (qAccordion_VERSION_MAJOR 0)
set (qAccordion_VERSION_MINOR 1)
set (qAccordion_VERSION_PATCH 0)

add_subdirectory(test)
add_subdirectory(include/qAccordion)
add_subdirectory(src)

