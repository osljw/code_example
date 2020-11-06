#include "include/ThreadPool.h"

ThreadPool::ThreadPool(size_t threads_count, AffinityType affinity_type, int core_index)
    : _stop(false), _core_index(0) {
    //unsigned num_cpus = std::thread::hardware_concurrency();

    for (size_t i = 0; i < threads_count; ++i) {
        std::thread t = std::thread(
            [this] {
                for(;;) {
                    std::function<void()> task;
                    {
                        std::unique_lock<std::mutex> lock(this->_queue_mutex);
                        this->_condition.wait(lock,
                                [this]{return this->_stop || !this->_tasks.empty();}
                                );
                        if (this->_stop && this->_tasks.empty()) return;

                        task = std::move(this->_tasks.front());
                        this->_tasks.pop();
                    }

                    task();
                }
            });

//        if (affinity_type != AFFINITY_DISABLE) {
//            int index = _core_index;
//            cpu_set_t cpuset;
//            CPU_ZERO(&cpuset);
//            if (affinity_type == AFFINITY_AVERAGE) {
//                index = _core_index++ % num_cpus;
//                if (core_index >= 0 && index == core_index) {
//                    index = _core_index++ % num_cpus;
//                }
//            } else if (affinity_type == AFFINITY_SINGLE) {
//                if (index >= num_cpus || index < 0) {
//                    index = 0;
//                }
//            }
//
//            std::vector<int> cores = { index };
//            SetThreadAffinity(t, cores);
//        }
        _workers.emplace_back(std::move(t));
    }
}


ThreadPool::~ThreadPool()
{
    {
      std::unique_lock<std::mutex> lock(_queue_mutex);
      _stop = true;
    }
    _condition.notify_all();
    for(std::thread& worker : _workers) {
      worker.join();
    }
}



size_t ThreadPool::task_size() {
    return _tasks.size();
}

