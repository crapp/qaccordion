# Initialize CXXFLAGS for Linux and OS X
if (NOT WIN32)
    set(CMAKE_CXX_FLAGS                "${CMAKE_CXX_FLAGS} -Wall -std=c++11")
    set(CMAKE_CXX_FLAGS_DEBUG          "-O0 -g")
    # No reorder warnings because these pop up frequently if you are using the Q_OBJECT macro
    set(CMAKE_CXX_FLAGS_DEBUG         "${CMAKE_CXX_FLAGS_DEBUG} -Wextra -Wno-reorder")
    set(CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    set(CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
endif(NOT WIN32)

# Compiler-specific C++11 activation.
if (${CMAKE_CXX_COMPILER_ID} MATCHES "GNU")
    execute_process(
        COMMAND ${CMAKE_CXX_COMPILER} -dumpversion OUTPUT_VARIABLE GCC_VERSION)
    if (NOT (GCC_VERSION VERSION_GREATER 4.7 OR GCC_VERSION VERSION_EQUAL 4.7))
        message(FATAL_ERROR "${PROJECT_NAME} requires g++ 4.7 or greater.")
    endif ()
elseif (${CMAKE_CXX_COMPILER_ID} MATCHES "Clang")
    if (${CMAKE_SYSTEM_NAME} MATCHES "Linux")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libstdc++")
    elseif (${CMAKE_SYSTEM_NAME} MATCHES "Darwin")
        set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")
    endif()
elseif(WIN32)
    # So far only msvc is supported on Windows.
    if (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")
    message(
      ERROR
      "qAccordion does not support compiling on Windows with "
      "${CMAKE_CXX_COMPILER_ID}. Please use MSVC.")
    endif (NOT CMAKE_CXX_COMPILER_ID MATCHES "MSVC")

    # MSVC 1800 supports C++11; earlier versions don't. So, warn if you try to
    # use anything else.
    if (${MSVC_VERSION} LESS 1800)
    message(
      ERROR
      "qAccordion does not support compiling on MSVC versions earlier than 1800. "
      "Please use MSVC 1800 (included with Visual Studio 2013 or later).")
    endif (${MSVC_VERSION} LESS 1800)
else ()
    message(FATAL_ERROR "Your C++ compiler does not support C++11.")
endif (WIN32)

# include GenerateExportHeader module
include(GenerateExportHeader)

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
endif(QACCORDION_EXTERNAL)

include_directories(
    ${PROJECT_BINARY_DIR} # include this directory so all moc headers and wrapped ui files are found
    ${base_path}/include
)
  
# configure a header file to pass some of the CMake settings
# to the source code
configure_file (
    ${base_path}/include/qAccordion/config.h.in
    ${CMAKE_CURRENT_BINARY_DIR}/config.h
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
add_library (qAccordion SHARED ${QACCORDION_HEADER} ${QACCORDION_SOURCE} ${ICON_RESOURCE_ADDED})
target_link_libraries(qAccordion Qt5::Widgets)

# generate the export header. this will be used to define a macro that exports functions on windows.
generate_export_header(qAccordion)

if (QACCORDION_BUILD_TESTER)
    # generate ui_*.h files
    qt5_wrap_ui(accordion_tester_FORMS ${TEST_UI})
    find_package(Qt5Network REQUIRED)
    add_executable(accordion_tester ${TEST_HEADER} ${TEST_SOURCE} ${accordion_tester_FORMS})
    target_link_libraries(accordion_tester qAccordion Qt5::Widgets Qt5::Network)
endif(QACCORDION_BUILD_TESTER)

install(TARGETS qAccordion DESTINATION lib)
install(FILES ${base_path}/include/qAccordion/clickableframe.h DESTINATION include/qAccordion)
install(FILES ${base_path}/include/qAccordion/contentpane.h DESTINATION include/qAccordion)
install(FILES ${base_path}/include/qAccordion/qaccordion.h DESTINATION include/qAccordion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/config.h DESTINATION include/qAccordion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/qaccordion_export.h DESTINATION include/qAccordion)
