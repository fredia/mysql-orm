#include <iostream>
#include <mysql/mysql.h>
#include "configuration.hpp"
#include "../test/configuration_test.hpp"
#include "../test/mysql_test.hpp"
#include "../test/connection_pool_test.hpp"
int main() {
    test_mysql_insert();
    return 0;
}