
#include<iostream>
#include<condition_variable>
#include<vector>
#include<thread>
 
std::vector<std::thread> tv;         //保存线程的情况
std::condition_variable_any m_t;     //条件变量
std::mutex lock;                     //互斥锁
int i = 1;                           //打印机资源 初始为1 表示可用
 
 
void subi() {            
    std::lock_guard<std::mutex> locker(lock);  //使用之前先加锁
    while(i == 0){             //如果不可用                
        m_t.wait(lock);        //将当前线程阻塞，注意：此时会释放锁
    }
 
    i--;                       //使用打印机过程
}
 
void addi() {                 
    std::lock_guard<std::mutex> locker(lock); //同理，放弃使用的时候先加锁
 
    i++;                       //是资源变为可用
    m_t.notify_all();          //通知其余阻塞的使用者可以使用了
}
 
void func(int j) {
    subi();
    std::cout << "I am thread "<< j << " , i = " << i << std::endl;
    addi();
}
 
 
int main(int argc,char *argv[])
{
    for(int j = 0; j < 10 ; ++j) {
        std::thread t(func,j);
        tv.push_back(std::move(t));
    }
 
    for(auto &thread : tv) {
        thread.join();
    }
    return 0;
}