#include <iostream>
#include <mysql/mysql.h>
#include "configuration.hpp"

using namespace std;

int main() {
    mysql_orm::configuration cfg{"127.0.0.1", "root", "000000", "test", 3000, 1};
    mysql_orm::config_manager::to_file(cfg, "mysql.cnf"sv);
    cout << mysql_orm::config_manager::get<string>("passwd", "mysql.cnf"sv);
    return 0;
}