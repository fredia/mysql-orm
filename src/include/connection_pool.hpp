//
// Created by fredia on 19-3-25.
//

#ifndef MYSQL_ORM_CONNECTION_POOL_HPP
#define MYSQL_ORM_CONNECTION_POOL_HPP

#include <mutex>
#include <condition_variable>
#include "configuration.hpp"
#include "mysql.hpp"

namespace mysql_orm {
    class connection_pool {
    public:
        static connection_pool &instance() {
            return instance_;  //懒汉模式
        }

        void init(int max_idle, int max_total) {

        }

        std::shared_ptr<mysql> get() {
            /*std::unique_lock<std::mutex> lock(mtx_);
            while ( pool_.empty() ){
                if(cv_.wait_for(lock, std::chrono::seconds(3))== std::cv_status::timeout){
                    return nullptr;
                }
            }

            auto conn = pool_.front();
            pool_.pop_front();
            lock.unlock();*/



        }

        void return_back(){

        }

    private:
        void expire(){

        }

    private:
        std::mutex mtx_;
        std::condition_variable cv_;
        std::once_flag once_flag_;
        std::deque<std::shared_ptr<mysql>> pool_;
        int max_total_;
        int max_idle_;
        static connection_pool instance_;
    };
}

#endif //MYSQL_ORM_CONNECTION_POOL_HPP
