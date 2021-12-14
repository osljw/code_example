#pragma once

#include "object.h"

class Apple : public Object {
public:
    Apple() {}
    ~Apple() {}

    std::string name() {
        return "apple";
    }
};

REGISTER_CLASS("apple", Apple)

//static Register<Apple> register_Apple("apple");