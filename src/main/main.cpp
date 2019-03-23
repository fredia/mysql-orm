#include <iostream>
#include <mysql/mysql.h>

using namespace std;

int main() {
    MYSQL mysql;
    mysql_init(&mysql);
    cout << "mysql is running" << endl;
    mysql_real_connect(&mysql, "localhost", "xxxx", "", "xxxx", 3306, NULL, 0);
    string sql = " insert into student(id, name) values('12345', 'test');";
    mysql_query(&mysql, sql.c_str());
    mysql_close(&mysql);
    return 0;
}