
#include <iostream>
#include <condition_variable>
#include <vector>
#include <mutex>
#include <thread>

class printer{

public:
    void sub_i(){
        std::unique_lock<std::mutex> lock(lock_);   //先加锁
        while(i==0){
            m_t.wait(lock);                     //阻塞线程,会释放锁
        }
        i--;        //使用打印机
    }

    void add_i(){
        std::unique_lock<std::mutex> lock(lock_);
        i++;
        m_t.notify_all();   //通知其余阻塞线程打印机可用
    }

    void func(int j){
        sub_i();
        std::cout<<"I am the thread"<< j <<", i = "<<i<<std::endl;
        add_i();
    }

private:
    std::condition_variable m_t;
    std::mutex lock_;
    int i=1;    //默认打印器可用，i为0即为不可用
};

int main(int argc, char *argv[]){
    printer printerObj;
    std::vector<std::thread> threads_;
    for(int i=0;i<10;i++){
        std::thread t(&printer::func,&printerObj,i);
        threads_.push_back(std::move(t));
    }
    for(auto & thread: threads_)
    {
        thread.join();
    }
    return 0;
}
