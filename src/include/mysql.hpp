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
#include "type_mapping.hpp"
#include "qualifier.hpp"

namespace mysql_orm {
    class mysql {
    public:
        ~mysql() {
            disconnect();
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
            if (mysql_real_connect(con_, cfg.host.c_str(), cfg.user_name.c_str(), cfg.passwd.c_str(),
                                   cfg.db_name.c_str(), 0, nullptr, 0) == nullptr) {
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

        template<typename T, typename... Args>
        bool create_table(Args&&... args) {
            std::string sql = generate_create_table_sql<T>(std::forward<Args>(args)...);
            if (mysql_query(con_, sql.data())) {
                std::cerr << mysql_error(con_) << std::endl;
                return false;
            }
            return true;
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

        template<typename T>
        inline constexpr auto get_type_names() {
            constexpr auto SIZE = iguana::get_value<T>();
            std::array<std::string, SIZE> arr = {};
            iguana::for_each(T{}, [&](auto &item, auto i) {
                constexpr auto Idx = decltype(i)::value;
                using U = std::remove_reference_t<decltype(iguana::get<Idx>(std::declval<T>()))>;
                std::string s;
                s = mysql_orm::type_to_name(identity<U>{});
                arr[Idx] = s;
            });

            return arr;
        }

        template<typename... Args>
        inline void append_sql(std::string& sql, Args&&... args) {
            (append(sql, std::forward<Args>(args)),...); //逗号表达式展开
        }

        template<typename T>
        void append(std::string &sql, const T &str) {
            if constexpr(std::is_same_v<std::string, T> || std::is_same_v<std::string_view, T>) {
                if (str.empty())
                    return;
            } else {
                if (sizeof(str) == 0) {
                    return;
                }
            }
            sql += str;
            sql += " ";
        }

        template <typename... Args, typename Func, std::size_t... Idx>
        inline void for_each_func(const std::tuple<Args...>& t, Func&& f, std::index_sequence<Idx...>) {
            (f(std::get<Idx>(t)), ...);
        }

        template<typename... Args>
        inline auto sort_tuple(const std::tuple<Args...>& tp){
            if constexpr(sizeof...(Args)==2){
                auto [a, b] = tp;
                if constexpr(!std::is_same_v<decltype(a), orm_key>&&!std::is_same_v<decltype(a), orm_auto_key>)
                    return std::make_tuple(b, a);
                else
                    return tp;
            }
            else{
                return tp;
            }
        }

        template<typename T, typename... Args>
        string generate_create_table_sql(Args... args) {
            const auto type_name_arr = get_type_names<T>();
            constexpr auto name = iguana::get_name<T>();
            std::string sql = std::string("CREATE TABLE IF NOT EXISTS ") + name.data() + "(";
            auto arr = iguana::get_array<T>();
            constexpr auto SIZE = sizeof... (Args);
            auto_key_map_[name.data()] = "";

            using U = std::tuple<std::decay_t<Args>...>;
            if constexpr (SIZE > 0) {
                static_assert(!(iguana::has_type<orm_key, U>::value && iguana::has_type<orm_auto_key, U>::value),
                              "should only one key");
            }
            auto tp = sort_tuple(std::make_tuple(std::forward<Args>(args)...));
            const size_t arr_size = arr.size();
            for (size_t i = 0; i < arr_size; ++i) {
                auto field_name = arr[i];
                bool has_add_field = false;
                for_each_func(tp, [&sql, &i, &has_add_field, field_name, type_name_arr, name, this](auto item) {
                    if constexpr (std::is_same_v<decltype(item), orm_not_null>) {
                        if (item.fields.find(field_name.data()) == item.fields.end())
                            return;
                    } else {
                        if (item.fields != field_name.data())
                            return;
                    }

                    if constexpr (std::is_same_v<decltype(item), orm_not_null>) {
                        if (!has_add_field) {
                            append_sql(sql, field_name.data(), " ", type_name_arr[i]);
                        }
                        append_sql(sql, " NOT NULL");
                        has_add_field = true;
                    } else if constexpr (std::is_same_v<decltype(item), orm_key>) {
                        if (!has_add_field) {
                            append(sql, field_name.data(), " ", type_name_arr[i]);
                        }
                        append_sql(sql, " PRIMARY KEY");
                        has_add_field = true;
                    } else if constexpr (std::is_same_v<decltype(item), orm_auto_key>) {
                        if (!has_add_field) {
                            append(sql, field_name.data(), " ", type_name_arr[i]);
                        }
                        append_sql(sql, " AUTO_INCREMENT");
                        append_sql(sql, " PRIMARY KEY");
                        auto_key_map_[name.data()] = item.fields;
                        has_add_field = true;
                    } else if constexpr (std::is_same_v<decltype(item), orm_unique>) {
                        if (!has_add_field) {
                            append_sql(sql, field_name.data(), " ", type_name_arr[i]);
                        }

                        append_sql(sql, ", UNIQUE(", item.fields, ")");
                        has_add_field = true;
                    } else {
                        append_sql(sql, field_name.data(), " ", type_name_arr[i]);
                    }
                }, std::make_index_sequence<SIZE>{});

                if (!has_add_field) {
                    append_sql(sql, field_name.data(), " ", type_name_arr[i]);
                }

                if (i < arr_size - 1)
                    sql += ", ";
            }

            sql += ")";

            std::cout << sql << std::endl;

            return sql;
        }

    private:
        MYSQL *con_ = nullptr;
        MYSQL_STMT *stmt_ = nullptr;
        inline static std::map<std::string, std::string> auto_key_map_;

    };
}
#endif //MYSQL_ORM_MYSQL_HPP
