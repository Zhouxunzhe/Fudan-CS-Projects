cmake_minimum_required(VERSION 3.10)
project(myjql)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_subdirectory(src)
add_subdirectory(test)
add_subdirectory(driver)
add_subdirectory(main)