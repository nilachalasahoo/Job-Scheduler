#include"ThreadPool.h"
int main(){
   ThreadPool pools;
   thread_count++;
   std::thread pools_thread(&ThreadPool::create_tasks,std::ref(pools));
   pools.start();
   pools_thread.join();
   return 0;
}