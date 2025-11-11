#include "datastructure.h"
#include <mutex>
#include <format>
#include <stdexcept>

DataStructure::DataStructure(int m): m_fields(m){}

void DataStructure::set_field(int index, int value){
    switch (index){
        case 0:{
            std::unique_lock lock(m_mutex0);
            m_fields[0] = value;
            break;}
        case 1:{
            std::unique_lock lock(m_mutex1);
            m_fields[1] = value;
            break;}
        default:{
            throw std::out_of_range("Invalid field index.");}
    }
}
int DataStructure::get_field(int index) const{
    switch (index){
        case 0:{
            std::shared_lock lock(m_mutex0);
            return m_fields[0];}
        case 1:{
            std::shared_lock lock(m_mutex1);
            return m_fields[1];}
        default:{
            throw std::out_of_range("Invalid field index.");}
    }
}

DataStructure::operator std::string() const{
    std::shared_lock lock0(m_mutex0, std::defer_lock);
    std::shared_lock lock1(m_mutex1, std::defer_lock);
    std::lock(lock0, lock1);

    return std::format("Field0: {}, Field1: {}", m_fields[0], m_fields[1]);
}