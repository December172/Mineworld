#include "ThreadPool.h"
#include "Mineworld.h"

namespace Mineworld {
    ThreadPool::ThreadPool(int numThreads) : stop(false) {
        auto worker = [this]() {
                while (true){
                    std::function<void()> task;
                    /// Lock the queue and wait for a task to be available
                    {   
                        std::unique_lock<std::mutex> lock(this->queueMutex);

                        this->condition.wait(lock, [this]() { return this -> stop || ! this -> tasks.empty(); });
                        if (this->stop && this->tasks.empty()) {
                            return;
                        }

                        task = std::move(this->tasks.front());
                        this->tasks.pop();
                        /// After this block, lock is released
                    }
                    try {
                        task();
                    } catch (...) {
                        Mineworld::getLogger().error("Exception in thread pool task");
                    };
                }};
        for (int i = 0; i < numThreads; i++) {
            threads.push_back(std::thread(worker));
        }
    }

    ThreadPool::~ThreadPool() {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
        lock.unlock();

        condition.notify_all();
        for (auto& thread : threads) {
            if (thread.joinable()) {
                thread.join();
            }
        }
    }
}