cmake_minimum_required(VERSION 3.0.0)
project(wav2mp3 VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wav2mp3 main.cpp wavreader.cpp mp3writer.cpp)
target_link_libraries(wav2mp3 libmp3lame.a)
target_include_directories(wav2mp3 PRIVATE /usr/local/include)
target_link_directories(wav2mp3 PRIVATE /usr/local/lib)

add_test(
    NAME test_no_param
    COMMAND wav2mp3
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set_tests_properties(
    test_no_param PROPERTIES
    PASS_REGULAR_EXPRESSION 
    "This tool converts a folder of wav files to mp3"
)

add_test(
    NAME test_with_param 
    COMMAND wav2mp3 samples
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set_tests_properties(
    test_with_param PROPERTIES
    PASS_REGULAR_EXPRESSION 
    "Hello, lame"
)

add_test(
    NAME test_with_nonexisting_folder 
    COMMAND wav2mp3 nonexisting
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
)
set_tests_properties(
    test_with_nonexisting_folder PROPERTIES
    PASS_REGULAR_EXPRESSION 
    "Folder does not exist:"
)


set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
