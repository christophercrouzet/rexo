cmake_minimum_required(VERSION 3.1)

project(Rexo
    VERSION 0.1.0
    LANGUAGES C)

# ------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# ------------------------------------------------------------------------------

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 98)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(
        -Wpedantic -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual
        -Wconversion -Wfloat-equal -Wpointer-arith -Wshadow -Wstrict-overflow=5
        -Wswitch -Wswitch-default -Wundef -Wunreachable-code -Wwrite-strings)
endif()

if(UNIX)
    add_definitions(-D_POSIX_C_SOURCE=199309L)
endif()

# ------------------------------------------------------------------------------

add_library(rexo INTERFACE)
target_include_directories(rexo
    INTERFACE
        "$<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>"
        "$<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>")
install(
    TARGETS rexo
    EXPORT ${PROJECT_NAME}Targets)
add_library(${PROJECT_NAME}::rexo ALIAS rexo)

if(UNIX)
    target_link_libraries(rexo INTERFACE m)
endif()

# ------------------------------------------------------------------------------

set(RX_TEST_TARGETS)

macro(rx_add_test)
    set(RX_ADD_TEST_OPTIONS)
    set(RX_ADD_TEST_SINGLE_VALUE_ARGS NAME)
    set(RX_ADD_TEST_MULTI_VALUE_ARGS FILES DEPENDS)
    cmake_parse_arguments(
        RX_ADD_TEST
        "${RX_ADD_TEST_OPTIONS}"
        "${RX_ADD_TEST_SINGLE_VALUE_ARGS}"
        "${RX_ADD_TEST_MULTI_VALUE_ARGS}"
        ${ARGN})

    add_executable(test-${RX_ADD_TEST_NAME} ${RX_ADD_TEST_FILES})
    set_target_properties(test-${RX_ADD_TEST_NAME}
        PROPERTIES
            RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin/tests
            OUTPUT_NAME ${RX_ADD_TEST_NAME})
    target_link_libraries(test-${RX_ADD_TEST_NAME}
        PRIVATE ${RX_ADD_TEST_DEPENDS})
    add_test(
        NAME ${RX_ADD_TEST_NAME}
        COMMAND test-${RX_ADD_TEST_NAME})
    list(APPEND RX_TEST_TARGETS test-${RX_ADD_TEST_NAME})
endmacro()

rx_add_test(
    NAME conformance
    FILES tests/conformance.c
    DEPENDS rexo)

add_custom_target(tests DEPENDS ${RX_TEST_TARGETS})

# ------------------------------------------------------------------------------

install(
    DIRECTORY include
    DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

# ------------------------------------------------------------------------------

set(RX_CMAKE_INSTALL_DIR ${CMAKE_INSTALL_LIBDIR}/cmake/${PROJECT_NAME})

configure_package_config_file(
    cmake/Config.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
    INSTALL_DESTINATION ${RX_CMAKE_INSTALL_DIR})

write_basic_package_version_file(
    ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    VERSION ${PROJECT_VERSION}
    COMPATIBILITY AnyNewerVersion)

install(
    FILES
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}Config.cmake
        ${CMAKE_CURRENT_BINARY_DIR}/${PROJECT_NAME}ConfigVersion.cmake
    DESTINATION ${RX_CMAKE_INSTALL_DIR})

install(
    EXPORT ${PROJECT_NAME}Targets
    FILE ${PROJECT_NAME}Targets.cmake
    NAMESPACE ${PROJECT_NAME}::
    DESTINATION ${RX_CMAKE_INSTALL_DIR})
