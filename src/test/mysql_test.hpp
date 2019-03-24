//
// Created by fredia on 19-3-24.
//

#ifndef MYSQL_ORM_MYSQL_TEST_HPP
#define MYSQL_ORM_MYSQL_TEST_HPP

#include "configuration.hpp"
#include "mysql.hpp"
#include <string>
#include <assert.h>

void test_mysql_connection() {
    mysql_orm::configuration cfg{"127.0.0.1", "root", "000000", "test", 60, 1};
    mysql_orm::mysql mysql;
    mysql.connect(cfg);
}

#endif //MYSQL_ORM_MYSQL_TEST_HPP
