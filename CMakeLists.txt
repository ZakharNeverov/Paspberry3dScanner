cmake_minimum_required(VERSION 3.0.0)
project(Scanner VERSION 0.1.0 LANGUAGES C CXX)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include(CTest)
enable_testing()

add_executable(Scanner main.cpp UART.cpp Scanner.cpp)



set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
find_package(PkgConfig)
find_library(PIGPIO_LIBRARY pigpio)
pkg_check_modules(SERIAL libserial)

target_include_directories(Scanner PRIVATE ${SERIAL_INCLUDE_DIRS} ${PIGPIO_INCLUDE_DIRS})
target_link_libraries(Scanner PRIVATE ${SERIAL_LDFLAGS} ${PIGPIO_LIBRARY})
include(CPack)
