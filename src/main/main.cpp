#include <iostream>
#include <mysql/mysql.h>
#include "configuration.hpp"
#include "../test/configuration_test.hpp"
#include "../test/mysql_test.hpp"

int main() {
    test_config();
    test_mysql_connection();
    return 0;
}