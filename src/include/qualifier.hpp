//
// Created by fredia on 19-3-24.   数据库关键字 （修饰符 ： not null）
//

#ifndef MYSQL_ORM_QUALIFIER_HPP
#define MYSQL_ORM_QUALIFIER_HPP

#include <set>
#include <string>

struct orm_not_null {
    std::set<std::string> fields;
};

struct orm_key {
    std::string fields;
};

struct orm_auto_key {
    std::string fields;
};

struct orm_unique {
    std::string fields;
};

#endif //MYSQL_ORM_QUALIFIER_HPP
