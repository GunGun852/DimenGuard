# Run a valid control first. A missing compiler/header must NOT count as a pass.
set(case_build "${BINARY_ROOT}/${CASE_NAME}")
set(configure_args -S "${SOURCE_ROOT}/tests/compile_case" -B "${case_build}"
    -G "${GENERATOR}" "-DCMAKE_CXX_COMPILER=${CXX_COMPILER}"
    "-DDG_ROOT=${SOURCE_ROOT}" "-DDG_CASE=${CASE_NAME}")
if(GENERATOR_PLATFORM)
    list(APPEND configure_args -A "${GENERATOR_PLATFORM}")
endif()
if(GENERATOR_TOOLSET)
    list(APPEND configure_args -T "${GENERATOR_TOOLSET}")
endif()
if(MAKE_PROGRAM)
    list(APPEND configure_args "-DCMAKE_MAKE_PROGRAM=${MAKE_PROGRAM}")
endif()
if(NOT CONFIG)
    set(CONFIG Release)
endif()
foreach(negative IN ITEMS OFF ON)
    execute_process(COMMAND "${CMAKE_COMMAND}" ${configure_args}
        "-DDG_NEGATIVE=${negative}" RESULT_VARIABLE configured
        OUTPUT_VARIABLE configure_out ERROR_VARIABLE configure_err)
    if(NOT configured EQUAL 0)
        message(FATAL_ERROR "Cannot configure ${CASE_NAME}: ${configure_out}${configure_err}")
    endif()
    # Always recompile after switching the macro; timestamps may share one clock tick.
    execute_process(COMMAND "${CMAKE_COMMAND}" --build "${case_build}" --config "${CONFIG}" --clean-first
        RESULT_VARIABLE built OUTPUT_VARIABLE build_out ERROR_VARIABLE build_err)
    file(WRITE "${case_build}/result-${negative}.log" "${build_out}${build_err}")
    if(NOT negative AND NOT built EQUAL 0)
        message(FATAL_ERROR "Valid control failed for ${CASE_NAME}: ${build_out}${build_err}")
    elseif(negative AND built EQUAL 0)
        message(FATAL_ERROR "Invalid expression unexpectedly compiled: ${CASE_NAME}")
    endif()
endforeach()
message(STATUS "${CASE_NAME}: valid control compiled; invalid expression rejected")
