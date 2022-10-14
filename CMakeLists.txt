cmake_minimum_required(VERSION 3.15)

project(Rexo
    VERSION 0.2.3
    LANGUAGES C CXX)

# ------------------------------------------------------------------------------

include(CMakePackageConfigHelpers)
include(GNUInstallDirs)

# ------------------------------------------------------------------------------

set(CMAKE_C_STANDARD 99)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

if(MSVC)
    add_compile_options(/W4)
else()
    add_compile_options(
        -Wpedantic -Wall -Wextra -Waggregate-return -Wcast-align -Wcast-qual
        -Wconversion -Wfloat-equal -Wpointer-arith -Wshadow -Wstrict-overflow=5
        -Wswitch -Wswitch-default -Wundef -Wunreachable-code -Wwrite-strings
        -Wformat-nonliteral -fsanitize=undefined,address)
    add_link_options(
        -fsanitize=undefined,address
    )
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

if(APPLE)
    target_compile_options(rexo INTERFACE -Wno-implicit-function-declaration)
endif()

if(UNIX)
    target_link_libraries(rexo INTERFACE m)
endif()

# ------------------------------------------------------------------------------

option(REXO_BUILD_TESTS "Build the test targets for Rexo" ON)

if(REXO_BUILD_TESTS)
    set(RX_TEST_TARGETS)

    macro(rx_add_test)
        set(RX_ADD_TEST_OPTIONS)
        set(RX_ADD_TEST_SINGLE_VALUE_ARGS NAME)
        set(RX_ADD_TEST_MULTI_VALUE_ARGS FILES DEPENDS PROPERTIES)
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
                OUTPUT_NAME ${RX_ADD_TEST_NAME}
                ${RX_ADD_TEST_PROPERTIES})
        target_link_libraries(test-${RX_ADD_TEST_NAME}
            PRIVATE ${RX_ADD_TEST_DEPENDS})
        add_test(
            NAME ${RX_ADD_TEST_NAME}
            COMMAND test-${RX_ADD_TEST_NAME})
        list(APPEND RX_TEST_TARGETS test-${RX_ADD_TEST_NAME})
    endmacro()

    enable_testing()

    if(NOT MSVC)
        rx_add_test(
            NAME c89-assertion-coverage
            FILES tests/c89/assertion-coverage.c
            DEPENDS rexo
            PROPERTIES
                C_STANDARD 90)

        rx_add_test(
            NAME c89-assertion-failure-messages
            FILES tests/c89/assertion-failure-messages.c
            DEPENDS rexo
            PROPERTIES
                C_STANDARD 90)

        rx_add_test(
            NAME c89-config-inherit
            FILES tests/c89/config-inherit.c
            DEPENDS rexo
            PROPERTIES
                C_STANDARD 90)

        rx_add_test(
            NAME c89-fixture
            FILES tests/c89/fixture.c
            DEPENDS rexo
            PROPERTIES
                C_STANDARD 90)

        rx_add_test(
            NAME c89-fixture-void
            FILES tests/c89/fixture-void.c
            DEPENDS rexo
            PROPERTIES
                C_STANDARD 90)

        rx_add_test(
            NAME c89-minimal
            FILES tests/c89/minimal.c
            DEPENDS rexo
            PROPERTIES
                C_STANDARD 90)
    endif()

    rx_add_test(
        NAME cpp98-assertion-coverage
        FILES tests/cpp98/assertion-coverage.cpp
        DEPENDS rexo
        PROPERTIES
            CXX_STANDARD 98)

    rx_add_test(
        NAME cpp98-assertion-failure-messages
        FILES tests/cpp98/assertion-failure-messages.cpp
        DEPENDS rexo
        PROPERTIES
            CXX_STANDARD 98)

    rx_add_test(
        NAME cpp98-config-inherit
        FILES tests/cpp98/config-inherit.cpp
        DEPENDS rexo
        PROPERTIES
            CXX_STANDARD 98)

    rx_add_test(
        NAME cpp98-fixture
        FILES tests/cpp98/fixture.cpp
        DEPENDS rexo
        PROPERTIES
            CXX_STANDARD 98)

    rx_add_test(
        NAME cpp98-fixture-void
        FILES tests/cpp98/fixture-void.cpp
        DEPENDS rexo
        PROPERTIES
            CXX_STANDARD 98)

    rx_add_test(
        NAME cpp98-minimal
        FILES tests/cpp98/minimal.cpp
        DEPENDS rexo
        PROPERTIES
            CXX_STANDARD 98)

    rx_add_test(
        NAME assertion-coverage
        FILES tests/assertion-coverage.c
        DEPENDS rexo)

    rx_add_test(
        NAME assertion-failure-messages
        FILES tests/assertion-failure-messages.c
        DEPENDS rexo)

    rx_add_test(
        NAME config-inherit
        FILES tests/config-inherit.c
        DEPENDS rexo)

    rx_add_test(
        NAME config-skip
        FILES tests/config-skip.c
        DEPENDS rexo)

    rx_add_test(
        NAME empty
        FILES tests/empty.c
        DEPENDS rexo)

    rx_add_test(
        NAME explicit
        FILES tests/explicit.c
        DEPENDS rexo)

    rx_add_test(
        NAME fixture
        FILES tests/fixture.c
        DEPENDS rexo)

    rx_add_test(
        NAME fixture-data-only
        FILES tests/fixture-data-only.c
        DEPENDS rexo)

    rx_add_test(
        NAME fixture-void
        FILES tests/fixture-void.c
        DEPENDS rexo)

    rx_add_test(
        NAME minimal
        FILES tests/minimal.c
        DEPENDS rexo)

    rx_add_test(
        NAME no-discovery
        FILES tests/no-discovery.c
        DEPENDS rexo)

    rx_add_test(
        NAME semi-explicit
        FILES tests/semi-explicit.c
        DEPENDS rexo)

    add_custom_target(tests DEPENDS ${RX_TEST_TARGETS})
endif()

# ------------------------------------------------------------------------------

install(
    DIRECTORY include/
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
