CMAKE_MINIMUM_REQUIRED(VERSION 3.5)
PROJECT(qAccordion VERSION 0.2.2)

option (QACCORDION_BUILD_TESTER OFF)
option (QACCORDION_EXTERNAL OFF)

# let cmake handle all the export business on windows
set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

add_subdirectory(test)
add_subdirectory(include/qaccordion)
add_subdirectory(src)

# use ext_include in parent projects with include directrories. makes sure the
# generated headers are found
if (QACCORDION_EXTERNAL)
    set (EXT_INCLUDE ${EXTERNAL_INCLUDE} PARENT_SCOPE)
endif(QACCORDION_EXTERNAL)
