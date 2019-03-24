//
// Created by fredia on 19-3-23.
//

#ifndef MYSQL_ORM_MYSQL_HPP
#define MYSQL_ORM_MYSQL_HPP

#include <vector>
#include<string>
#include <map>
#include<mysql/mysql.h>
#include <iostream>
#include <tuple>
#include "configuration.hpp"

namespace mysql_orm {
    class mysql {
    public:
        ~mysql() {


        }

        bool connect(mysql_orm::configuration &cfg) {
            if (con_ != nullptr) {
                mysql_close(con_);
            }

            con_ = mysql_init(nullptr);
            if (con_ == nullptr) {
                std::cerr << "mysql init failed\n";
                return false;
            }

            int timeout = cfg.timeout; //seconds unit
            if (mysql_options(con_, MYSQL_OPT_CONNECT_TIMEOUT, &timeout) != 0) {
                std::cerr << mysql_error(con_) << std::endl;
                return false;
            }
            char value = 1;
            if (mysql_options(con_, MYSQL_OPT_RECONNECT, &value) != 0) {
                std::cerr << mysql_error(con_) << std::endl;
                return false;
            }
            if (mysql_options(con_, MYSQL_SET_CHARSET_NAME, "utf8") != 0) {
                std::cerr << mysql_error(con_) << std::endl;
                return false;
            }
            if (mysql_real_connect(con_, cfg.host.c_str(), cfg.user_name.c_str(), cfg.passwd.c_str(), cfg.db_name.c_str(), 0, nullptr, 0) == nullptr) {
                std::cerr << mysql_error(con_) << std::endl;
                return false;
            }
            return true;
        }

        bool disconnect() {
            if (con_ != nullptr) {
                mysql_close(con_);
                con_ = nullptr;
            }
            return true;
        }

        bool create_database() {
            return false;

        }

        bool create_table() {
            return false;

        }

        int insert() {

            return 0;
        }

        int update() {
            return 0;

        }

        int batch_insert() {

            return 0;
        }

        int batch_update() {
            return 0;

        }

        bool delete_records() {
            return 0;
        }

        template<typename T>
        std::vector<T> query() {
            return nullptr;
        }

        bool execute() {
            return false;

        }

    private:


    private:
        MYSQL *con_ = nullptr;
        MYSQL_STMT *stmt_ = nullptr;
        inline static std::map<std::string, std::string> auto_key_map_;

    };
}
#endif //MYSQL_ORM_MYSQL_HPP
