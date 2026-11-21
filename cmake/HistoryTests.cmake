# Included by CliTests.cmake. Compare complete history sections, not the whole
# terminal transcript: evicted conversions still appear in earlier output.
function(check_history label input)
    check_interactive("history_${label}" "" "${input}" "Goodbye!")
    set(remaining "${interactive_output}")
    foreach(expected IN LISTS ARGN)
        string(FIND "${remaining}" "\nRecent successful conversions (" start)
        if(start EQUAL -1)
            message(FATAL_ERROR "History session ${label}: missing history section\n${remaining}")
        endif()
        string(SUBSTRING "${remaining}" ${start} -1 remaining)
        string(FIND "${remaining}" "\n1. Convert units\n" finish)
        if(finish EQUAL -1)
            message(FATAL_ERROR "History session ${label}: menu did not return\n${remaining}")
        endif()
        string(SUBSTRING "${remaining}" 0 ${finish} actual)
        if(NOT "${actual}" STREQUAL "${expected}")
            message(FATAL_ERROR "History session ${label}:\nExpected:${expected}\nActual:${actual}")
        endif()
        string(SUBSTRING "${remaining}" ${finish} -1 remaining)
    endforeach()
    string(FIND "${remaining}" "\nRecent successful conversions (" extra)
    if(NOT extra EQUAL -1)
        message(FATAL_ERROR "History session ${label}: unexpected extra history section")
    endif()
    set(interactive_output "${interactive_output}" PARENT_SCOPE)
endfunction()

set(empty_history "\nRecent successful conversions (0/10, oldest first):\nNo successful conversions in this session.\n")
check_history(empty "5\n0\n" "${empty_history}")

# Record varied successful results in order, including negative and zero values.
check_history(successful "1\n1\nmi\nm\n1\n72\nkm/h\nm/s\n1\n-2\nyd\nm\n1\n0\ns\nms\n5\n0\n"
    "\nRecent successful conversions (4/10, oldest first):\n1. 1 mi = 1609.344 m\n2. 72 km/h = 20 m/s\n3. -2 yd = -1.8288 m\n4. 0 s = 0 ms\n")

# Check both the exact capacity and repeated eviction of the oldest entry.
set(ten_conversions "")
set(ten_history "\nRecent successful conversions (10/10, oldest first):\n")
foreach(value RANGE 1 10)
    math(EXPR converted "${value} * 100")
    string(APPEND ten_conversions "1\n${value}\nm\ncm\n")
    string(APPEND ten_history "${value}. ${value} m = ${converted} cm\n")
endforeach()
set(last_ten_history "\nRecent successful conversions (10/10, oldest first):\n")
foreach(value RANGE 3 12)
    math(EXPR number "${value} - 2")
    math(EXPR converted "${value} * 100")
    string(APPEND last_ten_history "${number}. ${value} m = ${converted} cm\n")
endforeach()
check_history(capacity "${ten_conversions}5\n1\n11\nm\ncm\n1\n12\nm\ncm\n5\n0\n"
    "${ten_history}" "${last_ten_history}")

# Failures must neither create records nor evict successful records at capacity.
set(failed_attempts "9\n1\nabc\n1\n1abc\n1\nnan\n1\ninf\n1\n1e999\n1\n1\nbananas\nm\n1\n1\nm\nbananas\n1\n1\nm\ns\n1\n1e308\nkm\ncm\n")
check_history(failures "1\n1\nft\nm\n${failed_attempts}1\n1\nmi\nyd\n5\n0\n"
    "\nRecent successful conversions (2/10, oldest first):\n1. 1 ft = 0.3048 m\n2. 1 mi = 1760 yd\n")
check_history(failures_at_capacity "${ten_conversions}${failed_attempts}5\n0\n" "${ten_history}")

# Clear, reuse, and clear twice; clearing an empty history is harmless.
check_history(clear "1\n2\nm\ncm\n1\n60\ns\nmin\n5\n6\n5\n1\n500\ng\nkg\n5\n6\n6\n5\n0\n"
    "\nRecent successful conversions (2/10, oldest first):\n1. 2 m = 200 cm\n2. 60 s = 1 min\n"
    "${empty_history}"
    "\nRecent successful conversions (1/10, oldest first):\n1. 500 g = 0.5 kg\n"
    "${empty_history}")
string(FIND "${interactive_output}" "Conversion history cleared." cleared)
if(cleared EQUAL -1)
    message(FATAL_ERROR "Clear history did not confirm the action")
endif()

# Identical conversions count separately. Viewing history does not add entries.
set(duplicate_history "\nRecent successful conversions (2/10, oldest first):\n1. 1 yd = 3 ft\n2. 1 yd = 3 ft\n")
check_history(duplicates "1\n1\nyd\nft\n1\n1\nyd\nft\n5\n5\n0\n"
    "${duplicate_history}" "${duplicate_history}")

# This is a new process immediately after the non-empty duplicates session.
# Listings, demo calculations and explanations do not create history records.
check_history(new_session "2\n3\n4\n5\n0\n" "${empty_history}")
