function(check_cli expected_exit expected_text)
    execute_process(COMMAND "${PROGRAM}" ${ARGN} RESULT_VARIABLE code
        OUTPUT_VARIABLE out ERROR_VARIABLE err)
    if(NOT "${code}" STREQUAL "${expected_exit}")
        message(FATAL_ERROR "Unexpected exit ${code}: ${ARGN}\n${out}${err}")
    endif()
    string(FIND "${out}${err}" "${expected_text}" found)
    if(found EQUAL -1)
        message(FATAL_ERROR "Missing '${expected_text}' for ${ARGN}: ${out}${err}")
    endif()
endfunction()
check_cli(0 "72 km/h = 20 m/s" convert 72 km/h m/s)
check_cli(0 "1 ft = 0.3048 m" convert 1 ft m)
check_cli(0 "500 g = 0.5 kg" convert 500 g kg)
check_cli(0 "-1 m = -100 cm" convert -1 m cm)
check_cli(0 "1000 ms = 1 s" convert 1e3 ms s)
check_cli(2 "Incompatible dimensions" convert 1 m s)
check_cli(2 "Unknown unit" convert 1 bananas m)
check_cli(2 "Error:" convert nan m cm)
check_cli(2 "Error:" convert inf m cm)
check_cli(2 "Error:" convert 1abc m cm)
check_cli(2 "Error:" convert 1e999 m cm)
check_cli(2 "exceeds finite double range" convert 1e308 km cm)
check_cli(2 "Usage:" convert 1 m)
check_cli(2 "Usage:" unknown)
check_cli(0 "DimenGuard 1.0.0" --version)
check_cli(0 "km/h" units)

# Script stdin to test real interactive sessions, including recovery and EOF.
function(check_interactive label mode input)
    set(input_dir "${CMAKE_CURRENT_BINARY_DIR}/interactive-inputs")
    file(MAKE_DIRECTORY "${input_dir}")
    set(input_file "${input_dir}/${label}.txt")
    file(WRITE "${input_file}" "${input}")
    execute_process(COMMAND "${PROGRAM}" ${mode} INPUT_FILE "${input_file}"
        TIMEOUT 10 RESULT_VARIABLE code OUTPUT_VARIABLE out ERROR_VARIABLE err)
    if(NOT "${code}" STREQUAL "0")
        message(FATAL_ERROR "Interactive session ${label} exited ${code}: ${out}${err}")
    endif()
    foreach(expected IN LISTS ARGN)
        string(FIND "${out}${err}" "${expected}" found)
        if(found EQUAL -1)
            message(FATAL_ERROR "Interactive session ${label} missing '${expected}': ${out}${err}")
        endif()
    endforeach()
endfunction()

check_interactive(repeated "" "1\n72\nkm/h\nm/s\n1\n1\nft\nm\n1\n-2\nm\ncm\n0\n"
    "72 km/h = 20 m/s" "1 ft = 0.3048 m" "-2 m = -200 cm" "Goodbye!")
check_interactive(menu_options "" "2\n3\n4\n0\n"
    "m/s^2" "4 N" "DIMENSION LAB" "Goodbye!")
check_interactive(invalid_values "" "1\nabc\n1\n1abc\n1\nnan\n1\ninf\n1\n1e999\n1\n2\nm\ncm\n0\n"
    "VALUE must be a finite number" "VALUE is outside the supported number range" "2 m = 200 cm" "Goodbye!")
check_interactive(invalid_units "" "1\n1\nm\ns\n1\n1\nbananas\nm\n1\n1\nm\nbananas\n1\n1\nM\nm\n1\n500\ng\nkg\n0\n"
    "Incompatible dimensions" "Unknown unit 'bananas'" "Unknown unit 'M'" "500 g = 0.5 kg" "Goodbye!")
check_interactive(overflow "" "1\n1e308\nkm\ncm\n1\n1\nm\ncm\n0\n"
    "Conversion exceeds finite double range" "1 m = 100 cm" "Goodbye!")
check_interactive(whitespace "" " 1 \r\n 1e3 \r\n ms \r\n s \r\n 0 \r\n"
    "1000 ms = 1 s" "Goodbye!")
check_interactive(bad_choices "" "\n9\n1abc\n1\n\n0\n"
    "Please choose 0, 1, 2, 3, 4 or 5." "VALUE must be a finite number" "Goodbye!")
check_interactive(history "" "5\n1\n72\nkm/h\nm/s\n1\n500\ng\nkg\n5\n0\n"
    "No conversions yet." "Conversion history (2):" "1. 72 km/h = 20 m/s" "2. 500 g = 0.5 kg")
check_interactive(history_excludes_failures "" "1\n1\nm\ns\n5\n0\n"
    "Incompatible dimensions" "No conversions yet.")
check_interactive(eof_menu "" "" "Choose an option:" "Goodbye!")
check_interactive(eof_value "" "1\n" "Enter value:" "Goodbye!")
check_interactive(eof_from "" "1\n7\n" "From unit" "Goodbye!")
check_interactive(eof_to "" "1\n7\nm\n" "To unit:" "Goodbye!")
check_interactive(explicit_mode interactive "0\n" "Welcome to DimenGuard!" "Goodbye!")
message(STATUS "16 CLI cases and 14 interactive sessions passed")
