#include<iostream>
#include<thread>
#include<mutex>
const int max_thread=std::thread::hardware_concurrency();
int thread_count=0;
class Vector{
   private:
      int *array;
      int size;
      int capacity;
      std::mutex vector_mtx;

      void increase_size();
      void merger(int left_start,int left_end,int right_end);
      void merge_sort(int start,int end);
   public:
      Vector();
      bool empty();
      int length();
      void add(int element);
      void remove(int position);
      int operator[](int position);
      void sort();
      void reverse();
      void display();
      ~Vector();
};
Vector::Vector(){
   std::unique_lock<std::mutex> locker(vector_mtx);
   array=new int[25];
   for(int i=0;i<20;i++)
      array[i]=rand()%10000;
   size=20;
   capacity=25;
}
bool Vector::empty(){
   std::unique_lock<std::mutex> locker(vector_mtx);
   return size==0;
}
int Vector::length(){
   std::unique_lock<std::mutex> locker(vector_mtx);
   return size;
}
void Vector::increase_size(){
   int *new_array=new int[capacity+5];
   for(int i=0;i<size;i++){
      new_array[i]=array[i];
   }
   delete[] array;
   array=new_array;
   capacity=capacity+5;
}
void Vector::add(int element){
   std::unique_lock<std::mutex> locker(vector_mtx);
   if(size>=capacity)
      increase_size();
   array[size++]=element;
}
void Vector::remove(int position){
   std::unique_lock<std::mutex> locker(vector_mtx);
   if(position>=size)
      throw std::runtime_error("Index out of Bound ...");
   for(int i=position;i<size-1;i++){
      array[i]=array[i+1];
   }
   size--;
   array[size]=0;
}
int Vector::operator[](int position){
   std::unique_lock<std::mutex> locker(vector_mtx);
   if(position>=size)
      throw std::runtime_error("Index out of Bound ...");
   return array[position];
}
void Vector::merger(int left_start,int left_end,int right_end){
   int right_start=left_end+1;
   int *left_array=new int[left_end-left_start+1];
   int *right_array=new int[right_end-right_start+1];
   for(int i=left_start;i<=left_end;i++){
      left_array[i-left_start]=array[i];
   }
   for(int i=right_start;i<=right_end;i++){
      right_array[i-right_start]=array[i];
   }
   int start=left_start;
   left_end=left_end-left_start+1;
   left_start=0;
   right_end=right_end-right_start+1;
   right_start=0;
   while(left_start<left_end&&right_start<right_end){
        if(left_array[left_start]<right_array[right_start]){
            array[start++]=left_array[left_start++];
        }else{
            array[start++]=right_array[right_start++];
        }
    }
    while(left_start<left_end){
        array[start++]=left_array[left_start++];
    }
    while(right_start<right_end){
        array[start++]=right_array[right_start++];
    }
    delete[] left_array;
    delete[] right_array;
}
void Vector::merge_sort(int start,int end){
   if(start>=end)
      return;
   int mid=(end-start)/2;
   if(thread_count<max_thread){
        thread_count++;
        std::thread t1(&Vector::merge_sort,this,start,start+mid);
        thread_count++;
        std::thread t2(&Vector::merge_sort,this,start+mid+1,end);
        t1.join();
        thread_count--;
        t2.join();
        thread_count--;
    }else{
        merge_sort(start,start+mid);
        merge_sort(start+mid+1,end);
    }
    merger(start,start+mid,end);
}
void Vector::sort(){
   std::unique_lock<std::mutex> locker(vector_mtx);
    merge_sort(0,size-1);
}
void Vector::reverse(){
   std::unique_lock<std::mutex> locker(vector_mtx);
   int start=0;
   int end=size-1;
   while(start<end){
      int temp=array[start];
      array[start]=array[end];
      array[end]=temp;
      start++;
      end--;
   }
}
void Vector::display(){
   std::unique_lock<std::mutex> locker(vector_mtx);
   std::cout<<"The element in the vector are "<<std::endl;
   for(int i=0;i<size;i++){
      std::cout<<array[i]<<" ";
   }
   std::cout<<std::endl;
}
Vector::~Vector(){
   delete array;
}