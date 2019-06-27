#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock_guard, std::adopt_lock
#include <unistd.h>       //usleep()


std::mutex mtx;           // mutex for critical section

void print_thread_id (int id) {
//  mtx.lock();
  //std::lock_guard<std::mutex> lck(mtx, std::adopt_lock);
  std::lock_guard<std::mutex> lck(mtx);		//构造函数加锁,析构函数解锁,一个线程加锁之后,其余要求加锁的线程等待,指导一个结束.
  std::cout << "thread #" << id << '\n';
}

int main ()
{
  std::thread threads[10];
  // spawn 10 threads:
  for (int i=0; i<10; ++i)
    threads[i] = std::thread(print_thread_id,i+1);

  for (auto& th : threads) th.join();
  
  return 0;
}