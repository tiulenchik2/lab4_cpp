#include <vector>
#include <string>
#include <format>
#include <thread>
#include <iostream>
#include <mutex>
class MyStruct{
public:
    MyStruct(int m) : m_fields(m){}

    void set_field(int index, int value){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_fields[index] = value;
    }
    int get_field(int index) const{
        std::lock_guard<std::mutex> lock(m_mutex);
        return m_fields[index];
    }
    operator std::string() const{
        std::lock_guard<std::mutex> lock(m_mutex);
        return std::format("Field 0: {}, Field 1: {}", m_fields[0], m_fields[1]);
    }
    void increment_field(int index){
        std::lock_guard<std::mutex> lock(m_mutex);
        m_fields[index]++;
    }
private:
    std::vector<int> m_fields;
    mutable std::mutex m_mutex;
};

void worker_func(MyStruct* structure){
    for (int i = 0; i < 1000000; ++i){
        structure->increment_field(0);
    }
    
}

int main(){
    MyStruct mystruct(2);
    std::cout<<"Init state: "<<std::string(mystruct)<<"\n";
    std::cout<<"Start 2 threads..."<<"\n";
    
    std::thread t1(worker_func, &mystruct);
    std::thread t2(worker_func, &mystruct);
    t1.join();
    t2.join();
    
    std::cout<<"Expected 2000000."<<"\n";
    std::cout<<"Grantedd "<<mystruct.get_field(0)<<".\n";
    return 0;
}