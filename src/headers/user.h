#pragma once

#include <iostream>
#include <string>

class User {
    private:
    std::string name;
    int level;

    public:
    User();
    ~User();
    
    void setName(std::string valueName);
    std::string getName();

    void setLevel(int valueLevel);
    int getLevel();
    
};