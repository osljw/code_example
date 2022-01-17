#include <iostream>
#include <vector>

#include "vfunc_hook.h"

class Object{

public:
    // void print_test() {
    //     std::cout << "test" << std::endl;
    // }
    virtual void print_hello() {
        std::cout << "hello" << std::endl;
    }
};

void hk_print_hello(Object* obj) {
    std::cout << "hooked hello =========" << std::endl;
}

int main()
{

    std::cout << "size of std::uintptr: " << sizeof(std::uintptr_t) << std::endl;

    // Object obj;  will not work, because ob.print_hello is static build, not use 
    Object *obj = new Object;
    obj->print_hello();
    std::cout << "obj addr: " << obj << std::endl;


    std::uintptr_t **vbtl_ptr = reinterpret_cast<std::uintptr_t**>(obj);
    std::cout << "obj print_hello:" << (void*)(vbtl_ptr[0][0]) << std::endl;

    std::uintptr_t *vbtl = reinterpret_cast<std::uintptr_t*>(obj);
    std::cout << "obj vbtl ptr store: " << (void*)*vbtl << std::endl;

    vfunc_hook obj_hook;
    obj_hook.setup(obj, 5);
    obj_hook.hook_index<decltype(hk_print_hello)>(0, hk_print_hello);

    std::cout << "after hook, obj vbtl ptr store: " << (void*)*vbtl << std::endl;
    std::cout << "after hook, obj print_hello:" << (void*)(vbtl_ptr[0][0]) << std::endl;

    std::cout << "hk_print_hello addr:" << (void *)hk_print_hello << std::endl;
    

    obj->print_hello();
}