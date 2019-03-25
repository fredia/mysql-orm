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


struct autokey {
    int id;
    std::string name;
};
REFLECTION(autokey, id, name);

orm_auto_key autok{"id"};
orm_not_null not_null{{"id", "name"}};

void test_mysql_create_table() {
    mysql_orm::configuration cfg{"127.0.0.1", "test", "123456789", "test", 60, 1};
    mysql_orm::mysql mysql;
    mysql.connect(cfg);

    mysql.create_table<school>();
    mysql.create_table<autokey>(autok, not_null);

}

void test_mysql_insert() {

    mysql_orm::configuration cfg{"127.0.0.1", "test", "123456789", "test", 60, 1};
    mysql_orm::mysql mysql;
    mysql.connect(cfg);
    school school1{1, "BJTU", 3000};
    autokey ak{1, "auto"};

    assert(mysql.insert(school1) == 1);
    assert(mysql.insert(ak) == 1);

}

void test_mysql_batch_insert() {

    mysql_orm::configuration cfg{"127.0.0.1", "test", "123456789", "test", 60, 1};
    mysql_orm::mysql mysql;
    mysql.connect(cfg);


    vector<school> v;
    school school1{2, "NJU", 3000};
    school school2{3, "ZJU", 3000};
    v.push_back(std::move(school1));
    v.push_back(std::move(school2));
    assert(mysql.batch_insert(v) == 2);

}

void test_mysql_query() {
    mysql_orm::configuration cfg{"127.0.0.1", "test", "123456789", "test", 60, 1};
    mysql_orm::mysql mysql;
    mysql.connect(cfg);

    auto res = mysql.query<school>();
    std::cout << res.size() << std::endl;

    res = mysql.query<school>("where id = 1");
    std::cout << res.size() << std::endl;
}

void test_mysql_delete() {
    mysql_orm::configuration cfg{"127.0.0.1", "test", "123456789", "test", 60, 1};
    mysql_orm::mysql mysql;
    mysql.connect(cfg);
    school school1{2, "NJU", 3000};
    assert(mysql.insert(school1) == 1);
    std::cout << mysql.delete_records<school>("where id = 2") << std::endl;

}


#endif //MYSQL_ORM_MYSQL_TEST_HPP
