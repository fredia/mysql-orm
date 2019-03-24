//
// Created by fredia on 19-3-24.
//

#ifndef MYSQL_ORM_MYSQL_TEST_HPP
#define MYSQL_ORM_MYSQL_TEST_HPP

#include "configuration.hpp"
#include "mysql.hpp"
#include <assert.h>
#include "../iguana/json.hpp"

void test_mysql_connection() {
    mysql_orm::configuration cfg{"127.0.0.1", "test", "123456789", "test", 60, 1};
    mysql_orm::mysql mysql;
    mysql.connect(cfg);
}

struct school {
    int id;
    std::string name;
    int num;
};
REFLECTION(school, id, name, num);

void test_mysql_create_table() {
    mysql_orm::configuration cfg{"127.0.0.1", "test", "123456789", "test", 60, 1};
    mysql_orm::mysql mysql;
    mysql.connect(cfg);

    mysql.create_table<school>();

}

#endif //MYSQL_ORM_MYSQL_TEST_HPP
