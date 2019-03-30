# mysql-orm
对mysql api做了一个简单的包装，可以满足简单的curd，是介于orm和jdbcTemplate之间的一个东西，叫orm有点不合适。
序列化部分使用了[iguana](https://github.com/qicosmos/iguana).
## 目录
* [开发环境](#开发环境)
* [安装使用](#安装使用)
* [功能接口](#功能接口)
* [想改进的地方](#想改进的地方)

## 开发环境

- 操作系统：ubuntu 18.04 LTS
- IDE： CLion(集成cmake)
- 编译器：GCC 7.3.0
- 构建工具:make 4.1
- 测试(todo): CTest

## 安装使用
- 安装mysql
    
        sudo apt-get install mysql
        
- 下载使用

mysql-orm是header-only的，可以直接将hpp文件复制到项目中引用

或者编译安装后使用：

        git clone https://github.com/fredia/mysql-orm.git
        git submodule update --init
        cd mysql-orm
        mkdir build
        cd build
        cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Release ..
        make
        sudo make install
     
## 功能接口

    //建表
    template<typename T, typename... Args>
    bool create_table(Args &&... args);
    //增
    template<typename T>
    std::enable_if_t<iguana::is_reflection_v<T>, int> insert(const T &t);
    
    template<typename T>
    std::enable_if_t<iguana::is_reflection_v<T>, int> batch_insert(const std::vector<T> &t);
    //删
    template<typename T, typename... Args>
    std::enable_if_t<iguana::is_reflection_v<T>, int> delete_records(Args &&... args);
    //改
    template<typename T, typename... Args>
    std::enable_if_t<iguana::is_reflection_v<T>, int> update(const T &t, Args &&... args);
    //查
    template<typename T, typename... Args>
    std::enable_if_t<iguana::is_reflection_v<T>, std::vector<T>> query(Args &&... args);
    //DDL execute
    /**
      * 只支持没有占位符 的语句，可用作 drop table，drop database 等DDL
      * @return
      */
    bool execute(const std::string &sql)        

## 想改进的地方
- orm限制太多，想实现类似jdbcTemplate置=替换`？`的功能，支持带占位符的语句
- 实现类似jdbcTemplate自定义rowMapper的功能
- 支持多表查询
