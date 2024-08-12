#ifndef FILE_UTIL_THREAD_POOL_H
#define FILE_UTIL_THREAD_POOL_H

#include <vector>
#include <queue>
#include <thread>
#include <future>
#include <mutex>
#include <condition_variable>
#include <functional>
#include <stdexcept>

namespace Mineworld {
    class ThreadPool {
    public:
        ThreadPool(int numThreads);
        ~ThreadPool();

        template<typename Task, typename... Args>
        auto enqueue(Task&& task, Args&&... args) -> std::future<typename std::invoke_result<Task, Args...>::type> {
            using ReturnType = typename std::invoke_result<Task, Args...>::type;

            auto taskWrapper = std::make_shared<std::packaged_task<ReturnType()>>(std::bind(std::forward<Task>(task), std::forward<Args>(args)...));
            std::future<ReturnType> res = taskWrapper->get_future();
            {
                std::unique_lock<std::mutex> lock(queueMutex);
                if (stop) {
                    throw std::runtime_error("enqueue a task on stopped ThreadPool");
                }
                tasks.push([taskWrapper]() { (*taskWrapper)(); });
            }
            condition.notify_one();
            return res;
        };
    private:
        std::vector<std::thread> threads;
        std::queue<std::function<void()>> tasks;
        std::mutex queueMutex;
        std::condition_variable condition;
        bool stop;
    };
}

#endif // FILE_UTIL_THREAD_POOL_H