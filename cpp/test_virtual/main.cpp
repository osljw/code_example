#include <iostream>

class Object{

public:
    virtual void print_test() {
        std::cout << "test" << std::endl;
    }
    virtual void print_hello(int input) {
        std::cout << "hello: " << age << ", input: " << input << std::endl;
    }

private:
    int age = 66;
};

typedef void (*test_ptr)(Object*);
typedef void (*hello_ptr)(Object*, int);


int main()
{
    Object obj;
    std::uintptr_t **vbtl_ptr = reinterpret_cast<std::uintptr_t**>(&obj);
    ((test_ptr)vbtl_ptr[0][0])(nullptr);
    ((hello_ptr)vbtl_ptr[0][1])(&obj, 77);
    std::cout << "obj addr: " << &obj << std::endl;

    //Object *obj = new Object();
    // std::uintptr_t **vbtl_ptr = reinterpret_cast<std::uintptr_t**>(obj);
    // ((test_ptr)vbtl_ptr[0][0])(nullptr);
    // ((hello_ptr)vbtl_ptr[0][1])(obj, 77);

    std::cout << "vbtl_ptr: " << vbtl_ptr << std::endl;
    std::cout << "*vbtl_ptr: " << *vbtl_ptr << std::endl;
    
    std::cout << "f1: " << (void*)**vbtl_ptr << std::endl;
    std::cout << "f2: " << (void*)*(*vbtl_ptr + 1) << std::endl;

    std::uintptr_t *new_vbtl = new std::uintptr_t[2];
    new_vbtl[0] = vbtl_ptr[0][0];
    new_vbtl[1] = vbtl_ptr[0][1];

    //*(*vbtl_ptr + 1) = **vbtl_ptr;

    ((test_ptr)(new_vbtl[0]))(nullptr);
    //((hello_ptr)(*(*vbtl_ptr + 1)))(obj, 77);


    // ((test_ptr)vbtl_ptr[0][0])(nullptr);
    // ((hello_ptr)vbtl_ptr[0][1])(&obj, 77);

    return 0;
}
