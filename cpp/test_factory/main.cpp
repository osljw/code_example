#include <iostream>

#include "class_factory.h"
#include "apple.h"
#include "banana.h"

int main() {
    std::cout << "class registered: " << ClassFactory::Get().class_map_.size() << std::endl;

    auto apple = ClassFactory::create("apple");
    if (apple) {
        std::cout << "created: " << apple->name() << std::endl;
    }

    std::cout << "test end" << std::endl;
    return 0;
}
