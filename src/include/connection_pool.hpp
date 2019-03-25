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
            static connection_pool instance_;
            return instance_;
        }

        void init(int max_idle, configuration &cfg) {

            auto func = [max_idle, &cfg, this]() {
                this->max_idle_ = max_idle;
                this->cfg_ = cfg;
                for (int i = 0; i < max_idle; i++) {
                    std::shared_ptr<mysql> conn = std::make_shared<mysql>();
                    conn->connect(this->cfg_);
                    pool_.push_back(conn);
                    cv_.notify_one();
                }
            };
            std::call_once(once_flag_, func);
        }

        std::shared_ptr<mysql> get() {
            std::unique_lock<std::mutex> lock(mtx_);

            while (pool_.empty()) {
                if (cv_.wait_for(lock, std::chrono::seconds(3)) == std::cv_status::timeout) {
                    return nullptr;
                }
            }
            auto conn = pool_.front();
            pool_.pop_front();
            lock.unlock();
            return conn;
        }

        void return_back(std::shared_ptr<mysql> conn) {
            if (conn == nullptr) {
                conn = std::make_shared<mysql>();
                if (!(conn->connect(cfg_))) {
                    conn = nullptr;//连不上时 返回null  由调用者处理为null的情况
                }
            }
            std::unique_lock<std::mutex> lock(mtx_);
            pool_.push_back(conn);
            bool expired = expire();
            lock.unlock();
            if (!expired)
                cv_.notify_one();
        }

    private:
        bool expire() { //什么时机清除连接比较合适？ 搞个定时线程有点浪费，暂定在return_back()时清除
            if (pool_.size() > max_idle_) {
                pool_.pop_front();
                return true;
            }
            return false;
        }

        connection_pool() = default;

        ~connection_pool() = default;

        connection_pool(const connection_pool &) = delete;

        connection_pool &operator=(const connection_pool &) = delete;


    private:
        std::mutex mtx_;
        std::condition_variable cv_;
        std::once_flag once_flag_;
        std::deque<std::shared_ptr<mysql>> pool_;
        int max_idle_;
        configuration cfg_;
    };
}

#endif //MYSQL_ORM_CONNECTION_POOL_HPP
