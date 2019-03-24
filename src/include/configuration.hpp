//
// Created by fredia on 19-3-24.
//

#ifndef MYSQL_ORM_CONFIGURATION_HPP
#define MYSQL_ORM_CONFIGURATION_HPP

#include <string>
#include <fstream>
#include <string_view>
#include "../iguana/json.hpp"

namespace mysql_orm {
    struct configuration {
        std::string host;
        std::string user_name;
        std::string passwd;
        std::string db_name;
        int timeout;
        int db_conn_num;
    };
    REFLECTION(configuration, host, user_name, passwd, db_name, timeout, db_conn_num);

    class config_manager {
    public:
        config_manager() = delete;

        template<typename T, typename U>
        inline static void assign(T &t, U &u) {
            if constexpr(std::is_same_v<U, T>) {
                t = u;
            }
        }

        template<typename T>
        inline static bool from_file(T &t, std::string_view file_path) {
            std::ifstream in(file_path.data(), std::ios::binary);
            if (!in.is_open()) {
                return false;
            }

            in.seekg(0, std::ios::end);
            size_t len = (size_t) in.tellg();
            in.seekg(0);
            std::string str;
            str.resize(len);

            in.read(str.data(), len);

            bool r = iguana::json::from_json(t, str.data(), len);
            if (!r) {
                return false;
            }

            return true;
        }

        template<typename T>
        inline static bool to_file(T &t, std::string_view file_path) {
            iguana::string_stream ss;
            iguana::json::to_json(ss, t);
            std::ofstream out(file_path.data(), std::ios::binary);
            if (!out.is_open()) {
                std::cerr << "open  file failed";
                return false;
            }
            out.write(ss.str().data(), ss.str().size());
            out.close();
            return true;
        }

        //从文件读取某项数据库配置
        template<typename T>
        inline static T get(std::string_view key, std::string_view file_path) {
            configuration cfg{};
            bool r = from_file(cfg, file_path);
            if (!r) {
                std::cerr << "read from file failed!" << std::endl;
                return {};
            }
            T val{};
            bool has_key = false;
            iguana::for_each(cfg, [key, &val, &cfg, &has_key](const auto &item, auto I) {
                if (key == iguana::get_name<configuration>(decltype(I)::value)) {
                    assign(val, cfg.*item);
                    has_key = true;
                }
            });

            if (!has_key) {
                std::cerr << "do not have key" << std::endl;
                return {};
            }

            return val;
        }

        template<typename T>
        inline static bool set(std::string_view key, T &&val, std::string_view file_path) {
            configuration cfg{};
            bool r = from_file(cfg, file_path);
            if (!r) {
                std::cerr << "read from file failed!" << std::endl;
                return false;
            }

            bool has_key = false;
            iguana::for_each(cfg, [key, &val, &cfg, &has_key](auto &item, auto I) {
                if (key == iguana::get_name<configuration>(decltype(I)::value)) {
                    assign(cfg.*item, val);
                    has_key = true;
                }
            });

            if (!has_key) {
                std::cerr << "do not have key" << std::endl;
                return false;
            }
            return to_file(cfg, file_path);
        }

    };
}
#endif //MYSQL_ORM_CONFIGURATION_HPP
