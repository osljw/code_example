#pragma once

#include "object.h"

class Banana : public Object {
public:
    Banana() {}
    ~Banana() {}
};

REGISTER_CLASS("banana", Banana)