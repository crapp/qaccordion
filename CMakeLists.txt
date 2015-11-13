CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(qAccordion)

option (QACCORDION_BUILD_TESTER OFF)
option (QACCORDION_EXTERNAL OFF)

set (qAccordion_VERSION_MAJOR 0)
set (qAccordion_VERSION_MINOR 2)
set (qAccordion_VERSION_PATCH 1)

add_subdirectory(test)
add_subdirectory(include/qAccordion)
add_subdirectory(src)

# use ext_include in parent projects with include directrories. makes sure the generated headers are found
if (QACCORDION_EXTERNAL)
    set (EXT_INCLUDE ${EXTERNAL_INCLUDE} PARENT_SCOPE)
endif(QACCORDION_EXTERNAL)
