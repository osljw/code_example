#pragma once

#include "class_factory.h"

class Object {
public:
    virtual ~Object() {};

    virtual std::string name() {
        return "";
    }
};