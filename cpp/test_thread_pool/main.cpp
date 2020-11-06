#include <iostream>

#include "include/ThreadPool.h"
#include "blocking_counter.h"

using namespace std;

void print() {
    std::cout<<"123456"<<std::endl;
}


void ParallelFor(int batch_size, int desired_parallelism,
                 ThreadPool* thread_pool,
                 std::function<void(int, int)> do_work) {
  // Parallelize work over the batch.
  if (desired_parallelism <= 0) {
    do_work(0, batch_size);
    return;
  }
  const int num_shards = std::max<int>(
      1, std::min(static_cast<int>(desired_parallelism), batch_size));
  const int block_size = (batch_size + num_shards - 1) / num_shards;
  //CHECK_GT(block_size, 0);
  const int num_shards_used = (batch_size + block_size - 1) / block_size;
  BlockingCounter counter(num_shards_used - 1);
  for (int start = block_size; start < batch_size; start += block_size) {
    auto end = std::min(start + block_size, batch_size);
    //thread_pool->Schedule([&do_work, &counter, start, end]() {
    thread_pool->enqueue([&do_work, &counter, start, end]() {
      do_work(start, end);
      counter.DecrementCount();
    });
  }

  // Execute first shard on main thread.
  do_work(0, std::min(block_size, batch_size));
  counter.Wait();
}


int main()
{
    cout << "Hello world!" << endl;
    ThreadPool thread_pool(3, AffinityType::AFFINITY_AVERAGE, 0);

    //std::future<void> result = thread_pool.enqueue(print);


    //std::future<void> result = thread_pool.enqueue(std::move(print));

    auto print_work = [](int start, int end) {
        for(int i=start; i<end; i++) {
            std::cout<<i<<std::endl;
        }
    };


    ParallelFor(10, 3, &thread_pool, print_work);
    return 0;
}
