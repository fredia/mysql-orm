//
// Created by fredia on 19-3-25.
//

#ifndef MYSQL_ORM_CONNECTION_POOL_TEST_HPP
#define MYSQL_ORM_CONNECTION_POOL_TEST_HPP

#include "connection_pool.hpp"
#include "configuration.hpp"

void test_pool() {
    mysql_orm::connection_pool &pool = mysql_orm::connection_pool::instance();
    mysql_orm::configuration cfg{"127.0.0.1", "test", "123456789", "test", 60, 1};
    pool.init(2, cfg);

    auto conn1 = pool.get();

    auto conn2 = pool.get();

    auto conn3 = pool.get(); //null

}

#endif //MYSQL_ORM_CONNECTION_POOL_TEST_HPP
