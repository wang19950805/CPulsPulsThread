// ThreadProject.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include <iostream>
#include <thread>
#include <future>

#include "ThreadPool.h"


using namespace std;

int myfun(int a, int b) {
    cout << "my fun..." << endl;
    cout << "my fun thread id :"<<this_thread::get_id() << endl;
    return a + b;
}

void async_task(promise<int> *prom) {
    this_thread::sleep_for(chrono::seconds(1));
    prom->set_value(42);
}


int main()
{
    //async返回一个future对象
    //future<int> mf = async(myfun, 20, 30);

   /* aunch::async : 单独开启一个子线程执行
    aunch::deferred : 等待主线程进行执行*/
    future<int> mf2 = async(launch::async, myfun, 20, 30);
    cout << "main mf2 thread id :" << this_thread::get_id() << endl;
    future<int> mf3 = async(launch::deferred, myfun, 20, 30);
    cout << "main mf3 thread id :" << this_thread::get_id() << endl;

    //cout << "my = " << mf.get() << endl;

   /* thread t(myfun, 20, 30);
    t.join();*/

    promise<int> prom; 
    future<int> fu = prom.get_future();
    thread t(async_task, &prom);
    cout << "value = " << fu.get() << endl;
    t.join();

    //使用packaged_task封装匿名函数作为异步执行的任务,在子线程中执行
    packaged_task<int(int, int)> task([](int a, int b){
            
        return a + b;
        
        });
    //在主线程中获得子线程的返回值对象future
    future<int> f = task.get_future();
    //task在子线程中执行任务
    task(20, 30);
    //获得future对象中的值
    cout << f.get() << endl;


    //线程池的使用
    ThreadPool pool(4);
    auto result1 = pool.enqueue([](int a){
        return a;
        },42);

    auto result2 = pool.enqueue([](int a,int b) {
        return a + b;
        }, 10,20);

    cout << result1.get() << endl;
    cout << result2.get() << endl;

    return 0;

}

