#pragma once

#include <iostream>
#include <string>

class Background {
    private:
    std::string name;
    int level;

    public:
    Background();
    ~Background();
    
    void setName(std::string valueName);
    std::string getName();
    
};