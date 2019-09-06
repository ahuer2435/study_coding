#include <iostream>       // std::cout
#include <thread>         // std::thread
#include <mutex>          // std::mutex, std::lock_guard, std::adopt_lock
#include <unistd.h>


std::mutex mtx;           // mutex for critical section

void print_thread_id1 (int id) {
  //mtx.lock();
  //std::lock_guard<std::mutex> lck(mtx, std::adopt_lock);
  std::lock_guard<std::mutex> lck(mtx);   //构造函数加锁,析构函数解锁,一个线程加锁之后,其余要求加锁的线程等待,指导一个结束.
  std::cout << "guard thread #" << id << '\n';
  usleep(1000);
  //mtx.unlock();
}

void print_thread_id2 (int id) {
  //mtx.lock();
  //std::lock_guard<std::mutex> lck(mtx, std::adopt_lock);
  std::unique_lock<std::mutex> lck(mtx);   //构造函数加锁,析构函数解锁,一个线程加锁之后,其余要求加锁的线程等待,指导一个结束.
  std::cout << "unique thread #" << id << '\n';
  //mtx.unlock();
}


int main ()
{
  std::thread threads[200];
  // spawn 10 threads:
  std::cout << "aaaa" << std::endl;
  for (int i=0; i<100; ++i)
    threads[i] = std::thread(print_thread_id1,i+1);

  std::cout << "bbbb" << std::endl;
  for (int i=0; i<100; ++i)
    threads[i+100] = std::thread(print_thread_id2,i+1);
  std::cout << "cccc" << std::endl;

  for (auto& th : threads) th.join();

  return 0;
}