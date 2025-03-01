#include"Vector.h"
#include<queue>
#include<condition_variable>
#include <chrono>
class ThreadPool{
   private:
      Vector *head=new Vector();
      std::mutex pool_mtx;
      std::queue<std::thread> pool;
      std::queue<int> tasks;
      std::condition_variable cv;
   public:
      void create_tasks();
      void start();
      void run_thread();
      ~ThreadPool();
};


void ThreadPool::create_tasks(){
   while(true){
      std::unique_lock<std::mutex> locker(pool_mtx);
      cv.wait(locker,[this]{
         return tasks.empty();});
      for(int i=0;i<10;i++){
         tasks.push(rand()%5);   
      }
      std::cout<<"Job created sucessfully..."<<std::endl<<std::endl;
      cv.notify_all();
   }
}
void ThreadPool::start(){
   //5 is random number to addoing threads in pool
   for(int i=0;i<5;i++){
      thread_count++;
      pool.push(std::thread(&ThreadPool::run_thread,this));
   }
   for(int i=0;i<5;i++){
      pool.front().join();
      thread_count--;
      pool.pop();
   }
}
void ThreadPool::run_thread(){
   while(true){
      std::unique_lock<std::mutex> locker(pool_mtx);
      cv.wait(locker,[this]{
         return !tasks.empty();});
      int operation=tasks.front();
      if(operation==1){
         std::cout<<"Inserting Operation Start"<<std::endl;
         int value=rand()%10000;
         head->add(value);
         std::cout<<"The Value "<<value<<" inserted Sucessfully....."<<std::endl;
      }
      else if(operation==2){
         std::cout<<"Deletion Operation Start"<<std::endl;
         int index=rand()%head->length();
         head->remove(index);
         std::cout<<"From index "<<index<<" value deleted Sucessfully....."<<std::endl;
      }
      else if(operation==3){
         std::cout<<"Sorting Operation Start"<<std::endl;
         head->sort();
         std::cout<<"Sorting Done....."<<std::endl;
      }
      else if(operation==4){
         std::cout<<"Reverse Operation Start"<<std::endl;
         head->reverse();
         std::cout<<"Reversing Done....."<<std::endl;
      }
      else
         head->display();
      tasks.pop();
      std::this_thread::sleep_for(std::chrono::seconds(2));
      locker.unlock();
      cv.notify_all();
   }
}
ThreadPool::~ThreadPool(){
   delete head;
}