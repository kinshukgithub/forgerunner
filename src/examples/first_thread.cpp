#include<iostream>
#include<thread>

void worker(){
    std::cout<<"worker thread started";
}

int main(){
    std::cout<<"main thread started";
    std::thread t(worker);
    t.join();
    std::cout<<"main thread ended";
    return 0;
}