#include <user.h>

User::User() {
    this->name = "";
    this->level = 0;
}

void User::setName(std::string valueName) {
    if (!(valueName == "")) {
        this->name = valueName;
    } else {
        std::cout << "Error, name is not be null!\n";
    }
}

std::string User::getName(){
    return name;
}

void User::setLevel(int valueLevel) {
    this->level += valueLevel;
}

int User::getLevel() {
    return this->level;
}