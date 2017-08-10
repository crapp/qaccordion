# Initialize CXXFLAGS for Linux and OS X
if (NOT WIN32)
    set(CMAKE_CXX_FLAGS       "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wno-reorder")
endif(NOT WIN32)

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library. This has dependencies on QtGui and QtCore!
find_package(Qt5Widgets 5.2 REQUIRED)
message(STATUS "Found Qt version ${Qt5Widgets_VERSION_STRING}")

if (QACCORDION_EXTERNAL)
    set(base_path ${CMAKE_SOURCE_DIR}/external/qaccordion/)
else ()
    set(base_path ${CMAKE_SOURCE_DIR})
endif()

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    ${base_path}/include/qaccordion/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/qaccordion_config.hpp
)

set (QACCORDION_SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/clickableframe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/contentpane.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qaccordion.cpp
)

set(QACCORDION_ICON_RESOURCE "${base_path}/icons/qaccordionicons.qrc")
# add resource files so they can be compiled into the binary
qt5_add_resources(ICON_RESOURCE_ADDED ${QACCORDION_ICON_RESOURCE})

# we are building a shared library
add_library (qAccordion SHARED
    ${QACCORDION_HEADER}
    ${QACCORDION_SOURCE}
    ${ICON_RESOURCE_ADDED})

target_link_libraries(qAccordion Qt5::Widgets)

set_property(TARGET qAccordion PROPERTY CXX_STANDARD_REQUIRED ON)
set_property(TARGET qAccordion PROPERTY CXX_STANDARD 11)

target_include_directories(qAccordion
    PUBLIC ${CMAKE_CURRENT_BINARY_DIR}
    PUBLIC ${base_path}/include
)

if (QACCORDION_BUILD_TESTER)
    # generate ui_*.h files
    qt5_wrap_ui(accordion_tester_FORMS ${TEST_UI})
    find_package(Qt5Network REQUIRED)
    add_executable(accordion_tester ${TEST_HEADER} ${TEST_SOURCE} ${accordion_tester_FORMS})
    target_link_libraries(accordion_tester qAccordion Qt5::Widgets Qt5::Network)
    set_property(TARGET accordion_tester PROPERTY CXX_STANDARD_REQUIRED ON)
    set_property(TARGET accordion_tester PROPERTY CXX_STANDARD 11)
endif(QACCORDION_BUILD_TESTER)

install(TARGETS qAccordion DESTINATION lib)
install(FILES ${base_path}/include/qAccordion/clickableframe.h DESTINATION include/qAccordion)
install(FILES ${base_path}/include/qAccordion/contentpane.h DESTINATION include/qAccordion)
install(FILES ${base_path}/include/qAccordion/qaccordion.h DESTINATION include/qAccordion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h DESTINATION include/qAccordion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qaccordion_export.h DESTINATION include/qAccordion)
