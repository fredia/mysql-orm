//
// Created by fredia on 19-3-24.
//

#ifndef MYSQL_ORM_CONFIGURATION_TEST_HPP
#define MYSQL_ORM_CONFIGURATION_TEST_HPP

#include "configuration.hpp"
#include <string>
#include <string_view>
#include <assert.h>

using namespace std;

void test_config() {
    mysql_orm::configuration cfg{"127.0.0.1", "root", "000000", "test", 60, 1};
    mysql_orm::config_manager::to_file(cfg, "mysql.cnf"sv);
    assert(mysql_orm::config_manager::get<string>("passwd", "mysql.cnf"sv) == cfg.passwd);
}

#endif //MYSQL_ORM_CONFIGURATION_TEST_HPP
