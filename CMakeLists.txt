cmake_minimum_required(VERSION 3.13)
project(mysql_orm)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/include)
aux_source_directory(src/main dir_srcs)

add_executable(mysql_orm ${dir_srcs} src/include/mysql_orm.hpp src/include/configuration.hpp)
target_link_libraries(mysql_orm mysqlclient)