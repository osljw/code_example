#ifndef THREADPOOL_H
#define THREADPOOL_H

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <future>
#include <functional>
#include <stdexcept>
#include <iostream>
#include <condition_variable>

//#include <boost/lockfree/queue.hpp>
//#include "util/time_utils.h"
//#include "util/logger.h"
//#include "util/util.h"
//#include "util/concurrent_queue.h"

enum AffinityType { AFFINITY_DISABLE, AFFINITY_AVERAGE, AFFINITY_SINGLE };
enum TaskStatus { TASK_INITED = 0, TASK_FINISHED, TASK_EXPIRED };

class ThreadPool {
  public:
    ThreadPool(size_t threads_count, AffinityType affinity_type, int core_index);

    ~ThreadPool();

    ThreadPool(const ThreadPool&) = delete;
    ThreadPool(ThreadPool&&) = delete;
    ThreadPool& operator=(const ThreadPool&) = delete;
    ThreadPool& operator=(ThreadPool&&) = delete;

    template<class F, class... Args>
    std::future<typename std::result_of<F(Args...)>::type> enqueue(F&& f, Args&&... args) {
        using return_type = typename std::result_of<F(Args...)>::type;

        auto task = std::make_shared<std::packaged_task<return_type()>>(
              std::bind(std::forward<F>(f), std::forward<Args>(args)...));

        std::future<return_type> res = task->get_future();

        {
          std::unique_lock<std::mutex> lock(_queue_mutex);
          _tasks.emplace([task]() { (*task)(); });
        }
        _condition.notify_one();
        return res;
    };

    size_t task_size();

  private:
    std::vector<std::thread> _workers;
    std::queue<std::function<void()>> _tasks;
    std::mutex _queue_mutex;
    std::condition_variable _condition;
    std::atomic_bool _stop;
    std::atomic<int> _core_index;
    bool _enable_affinity;

};

#endif // THREADPOOL_H
