//
// Created by fredia on 19-3-24.
//

#ifndef MYSQL_ORM_TYPE_MAPPING_HPP
#define MYSQL_ORM_TYPE_MAPPING_HPP

#include <mysql/mysql.h>
#include <string>
#include <string_view>
#include<array>

using namespace std::string_view_literals;
namespace mysql_orm {
    template<class T>
    struct identity {
    };

#define REGISTER_TYPE(Type, Index)                                              \
    inline constexpr int type_to_id(identity<Type>) noexcept { return Index; } \
    inline constexpr auto id_to_type( std::integral_constant<std::size_t, Index > ) noexcept { Type res{}; return res; }

    REGISTER_TYPE(char, MYSQL_TYPE_TINY)

    REGISTER_TYPE(short, MYSQL_TYPE_SHORT)

    REGISTER_TYPE(int, MYSQL_TYPE_LONG)

    REGISTER_TYPE(float, MYSQL_TYPE_FLOAT)

    REGISTER_TYPE(double, MYSQL_TYPE_DOUBLE)

    REGISTER_TYPE(int64_t, MYSQL_TYPE_LONGLONG)

    inline int type_to_id(identity<std::string>) noexcept { return MYSQL_TYPE_VAR_STRING; }

    inline std::string id_to_type(std::integral_constant<std::size_t, MYSQL_TYPE_VAR_STRING>) noexcept {
        std::string res{};
        return res;
    }

    inline constexpr auto type_to_name(identity<char>) noexcept { return "TINYINT"sv; }

    inline constexpr auto type_to_name(identity<short>) noexcept { return "SMALLINT"sv; }

    inline constexpr auto type_to_name(identity<int>) noexcept { return "INTEGER"sv; }

    inline constexpr auto type_to_name(identity<float>) noexcept { return "FLOAT"sv; }

    inline constexpr auto type_to_name(identity<double>) noexcept { return "DOUBLE"sv; }

    inline constexpr auto type_to_name(identity<int64_t>) noexcept { return "BIGINT"sv; }

    inline auto type_to_name(identity<std::string>) noexcept { return "TEXT"sv; }

    template<size_t N>
    inline auto type_to_name(identity<std::array<char, N>>) noexcept {
        std::string s = "varchar(" + std::to_string(N) + ")";
        return s;
    }

}
#endif //MYSQL_ORM_TYPE_MAPPING_HPP
