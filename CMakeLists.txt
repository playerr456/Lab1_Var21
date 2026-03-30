cmake_minimum_required(VERSION 3.16)
project(lab1_variant21 C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(lab1
    main.c
    matrix.c
    field_info.c
    complex_number.c
)

add_executable(lab1_tests
    run_all_tests.c
    tests.c
    matrix.c
    field_info.c
    complex_number.c
)

enable_testing()
add_test(NAME all_tests COMMAND lab1_tests)
