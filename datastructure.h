#pragma once

#include <string>
#include <vector>
#include <shared_mutex>

class DataStructure{
public:
    DataStructure(int m);
    void set_field(int index, int value);
    int get_field(int index) const;
    operator std::string() const;
private:
    std::vector<int> m_fields;
    mutable std::shared_mutex m_mutex0;
    mutable std::shared_mutex m_mutex1;
};