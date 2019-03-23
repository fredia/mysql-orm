cmake_minimum_required(VERSION 3.13)
project(mysql_orm)

set(CMAKE_CXX_STANDARD 17)

add_executable(mysql_orm src/main/main.cpp src/include/mysql.hpp)