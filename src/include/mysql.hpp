//
// Created by fredia on 19-3-23.
//

#ifndef MYSQL_ORM_MYSQL_HPP
#define MYSQL_ORM_MYSQL_HPP

#include <vector>

namespace mysql_orm {
    class mysql {
    public:
        ~mysql() {

        }

        bool connect() {

        }

        bool disconnect() {

        }

        bool create_database() {

        }

        bool create_table() {

        }

        constexpr int insert() {

        }

        constexpr int update() {

        }

        constexpr int batch_insert() {

        }

        constexpr int batch_update() {

        }

        bool delete_records() {

        }

        template<typename T>
        std::vector<T> query() {

        }

        bool execute() {

        }

    };
}
#endif //MYSQL_ORM_MYSQL_HPP
