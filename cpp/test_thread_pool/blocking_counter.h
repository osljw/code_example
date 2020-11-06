#include <mutex>
#include <condition_variable>

class BlockingCounter {
 public:
  BlockingCounter(int initial_count)
      : state_(initial_count << 1), notified_(false) {
    //CHECK_GE(initial_count, 0);
    //DCHECK_EQ((initial_count << 1) >> 1, initial_count);
  }

  ~BlockingCounter() {}

  inline void DecrementCount() {
    unsigned int v = state_.fetch_sub(2, std::memory_order_acq_rel) - 2;
    if (v != 1) {
      //DCHECK_NE(((v + 2) & ~1), 0);
      return;  // either count has not dropped to 0, or waiter is not waiting
    }
    std::unique_lock<std::mutex> l(mu_);
    //mutex_lock l(mu_);
    //DCHECK(!notified_);
    notified_ = true;
    cond_var_.notify_all();
  }

  inline void Wait() {
    unsigned int v = state_.fetch_or(1, std::memory_order_acq_rel);
    if ((v >> 1) == 0) return;
    //mutex_lock l(mu_);
    std::unique_lock<std::mutex> l(mu_);
    while (!notified_) {
      cond_var_.wait(l);
    }
  }
  // Wait for the specified time, return false iff the count has not dropped to
  // zero before the timeout expired.
  inline bool WaitFor(std::chrono::milliseconds ms) {
    unsigned int v = state_.fetch_or(1, std::memory_order_acq_rel);
    if ((v >> 1) == 0) return true;
    //mutex_lock l(mu_);
    std::unique_lock<std::mutex> l(mu_);
    while (!notified_) {
      const std::cv_status status = cond_var_.wait_for(l, ms);
      if (status == std::cv_status::timeout) {
        return false;
      }
    }
    return true;
  }

 private:
  std::mutex mu_;
  std::condition_variable cond_var_;
  std::atomic<int> state_;  // low bit is waiter flag
  bool notified_;
};
