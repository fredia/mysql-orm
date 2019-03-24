#include <iostream>
#include <mysql/mysql.h>
#include "configuration.hpp"
#include "../test/configuration_test.hpp"
#include "../test/mysql_test.hpp"

int main() {
    test_mysql_create_table();
    return 0;
}