cmake_minimum_required(VERSION 3.13)
project(mysql_orm)

set(CMAKE_CXX_STANDARD 17)

include_directories(src/include)
aux_source_directory(src/main dir_srcs)

add_executable(mysql_orm ${dir_srcs} src/include/mysql.hpp src/include/configuration.hpp src/test/configuration_test.hpp src/test/mysql_test.hpp src/include/type_mapping.hpp src/include/qualifier.hpp)
target_link_libraries(mysql_orm mysqlclient)