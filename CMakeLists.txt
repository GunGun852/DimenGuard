cmake_minimum_required(VERSION 3.20)
project(DimenGuard VERSION 1.0.0 LANGUAGES CXX)

include(GNUInstallDirs)
include(CMakePackageConfigHelpers)

add_library(dimenguard INTERFACE)
add_library(DimenGuard::dimenguard ALIAS dimenguard)
target_compile_features(dimenguard INTERFACE cxx_std_20)
target_include_directories(dimenguard INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>)

option(DIMENGUARD_BUILD_EXAMPLES "Build demo and example applications" ON)
option(DIMENGUARD_BUILD_TESTS "Build core and compile-fail tests" ON)
option(DIMENGUARD_SANITIZERS "Enable ASan/UBSan for project executables on GCC/Clang" OFF)

function(dg_executable target source)
    add_executable(${target} ${source})
    target_link_libraries(${target} PRIVATE DimenGuard::dimenguard)
    set_target_properties(${target} PROPERTIES CXX_EXTENSIONS OFF)
    if(MSVC)
        target_compile_options(${target} PRIVATE /W4 /permissive- /utf-8)
    else()
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wpedantic)
        if(DIMENGUARD_SANITIZERS)
            target_compile_options(${target} PRIVATE -fsanitize=address,undefined -fno-omit-frame-pointer)
            target_link_options(${target} PRIVATE -fsanitize=address,undefined)
        endif()
    endif()
endfunction()

if(DIMENGUARD_BUILD_EXAMPLES)
    dg_executable(dimenguard_cli app/main.cpp)
    set_target_properties(dimenguard_cli PROPERTIES OUTPUT_NAME dimenguard)
    dg_executable(physics_example examples/physics.cpp)
    dg_executable(custom_unit_example examples/custom_unit.cpp)
endif()

if(DIMENGUARD_BUILD_TESTS)
    enable_testing()
    dg_executable(dimenguard_tests tests/test_core.cpp)
    add_test(NAME core COMMAND dimenguard_tests)
    if(DIMENGUARD_BUILD_EXAMPLES)
        add_test(NAME demo COMMAND dimenguard_cli demo)
        set_tests_properties(demo PROPERTIES PASS_REGULAR_EXPRESSION "4 N")
        add_test(NAME explanations COMMAND dimenguard_cli explain)
        set_tests_properties(explanations PROPERTIES PASS_REGULAR_EXPRESSION "T\\^-2")
        add_test(NAME physics COMMAND physics_example)
        set_tests_properties(physics PROPERTIES PASS_REGULAR_EXPRESSION "2400 N")
        add_test(NAME custom_unit COMMAND custom_unit_example)
        set_tests_properties(custom_unit PROPERTIES PASS_REGULAR_EXPRESSION "1 ft")
        add_test(NAME cli_cases COMMAND ${CMAKE_COMMAND}
            "-DPROGRAM=$<TARGET_FILE:dimenguard_cli>"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CliTests.cmake")
    endif()
    foreach(case IN ITEMS incompatible_add mixed_scale_add wrong_cast implicit_scalar
            implicit_conversion scalar_add incompatible_compare wrong_argument invalid_power
            invalid_dimension invalid_scale)
        add_test(NAME reject_${case} COMMAND ${CMAKE_COMMAND}
            "-DSOURCE_ROOT=${CMAKE_CURRENT_SOURCE_DIR}"
            "-DCASE_NAME=${case}"
            "-DBINARY_ROOT=${CMAKE_CURRENT_BINARY_DIR}/compile_cases"
            "-DCXX_COMPILER=${CMAKE_CXX_COMPILER}"
            "-DGENERATOR=${CMAKE_GENERATOR}"
            "-DGENERATOR_PLATFORM=${CMAKE_GENERATOR_PLATFORM}"
            "-DGENERATOR_TOOLSET=${CMAKE_GENERATOR_TOOLSET}"
            "-DMAKE_PROGRAM=${CMAKE_MAKE_PROGRAM}"
            "-DCONFIG=$<CONFIG>"
            -P "${CMAKE_CURRENT_SOURCE_DIR}/cmake/CompileCase.cmake")
    endforeach()
endif()

install(TARGETS dimenguard EXPORT DimenGuardTargets)
install(DIRECTORY include/ DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
install(EXPORT DimenGuardTargets NAMESPACE DimenGuard::
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DimenGuard)
configure_package_config_file(cmake/DimenGuardConfig.cmake.in
    ${CMAKE_CURRENT_BINARY_DIR}/DimenGuardConfig.cmake
    INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DimenGuard)
write_basic_package_version_file(${CMAKE_CURRENT_BINARY_DIR}/DimenGuardConfigVersion.cmake
    VERSION ${PROJECT_VERSION} COMPATIBILITY SameMajorVersion)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/DimenGuardConfig.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/DimenGuardConfigVersion.cmake
    DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/DimenGuard)
