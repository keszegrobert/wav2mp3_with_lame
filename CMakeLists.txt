cmake_minimum_required(VERSION 3.0.0)
project(wav2mp3 VERSION 0.1.0)

include(CTest)
enable_testing()

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(wav2mp3 main.cpp)
target_link_libraries(wav2mp3 libmp3lame.a)
target_include_directories(wav2mp3 PRIVATE /usr/local/include)
target_link_directories(wav2mp3 PRIVATE /usr/local/lib)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
