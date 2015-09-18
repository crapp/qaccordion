# Initialize CXXFLAGS.
set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
set(CMAKE_CXX_FLAGS_DEBUG          "-Wextra -O0 -g -Wno-reorder") # No reorder warnings since this pop up frequently if you are using the Q_OBJECT macro
set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")

# Compiler-specific C++11 activation.
if ("${CMAKE_CXX_COMPILER_ID}" MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif ("${CMAKE_CXX_COMPILER_ID}" MATCHES "Clang")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif ()

# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library. This has dependencies on QtGui and QtCore!
find_package(Qt5Widgets 5.2 REQUIRED)
message(STATUS "Found Qt version ${Qt5Widgets_VERSION_STRING}")

# include this directories so all moc headers and wrapped ui files are found
include_directories(
    ${PROJECT_BINARY_DIR}
    ${CMAKE_SOURCE_DIR}/include
)

# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    ${CMAKE_SOURCE_DIR}/include/qAccordion/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
)

set (SOURCE
    ${CMAKE_CURRENT_SOURCE_DIR}/clickableframe.cpp
    ${CMAKE_CURRENT_SOURCE_DIR}/qaccordion.cpp
)

set(ICON_RESOURCE ${PROJECT_SOURCE_DIR}/icons/icons.qrc)
# add resource files so they can be compiled into the binary
qt5_add_resources(ICON_RESOURCE_ADDED ${ICON_RESOURCE})

add_library (qAccordion SHARED ${HEADER} ${SOURCE} ${ICON_RESOURCE_ADDED})
target_link_libraries(qAccordion Qt5::Widgets)

if (BUILD_TESTER)
    # generate ui_*.h files
    qt5_wrap_ui(accordion_tester_FORMS ${TEST_UI})
    find_package(Qt5Network REQUIRED)
    add_executable(accordion_tester ${TEST_HEADER} ${TEST_SOURCE} ${accordion_tester_FORMS})
    target_link_libraries(accordion_tester qAccordion Qt5::Widgets Qt5::Network)
endif()
