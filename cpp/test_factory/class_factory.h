#pragma once

#include <unordered_map>
#include <functional>

#include "object.h"

// https://blog.csdn.net/weixin_44843859/article/details/109756277
class ClassFactory {
public:
    ClassFactory() {}

    static ClassFactory& Get() {
        static ClassFactory globalClassFactory;
        return globalClassFactory;
    }

    static Object* create(const std::string& class_name) {
        if (class_map_.count(class_name)) {
            return class_map_[class_name]();
        }
        return nullptr;
    }

    void RegisterClass(const std::string& class_name, std::function<Object*()> const& func) {
        class_map_[class_name] = func;
    }

//private:
    ClassFactory(const ClassFactory &) = delete;
    ClassFactory(ClassFactory &&) = delete;
    static std::unordered_map<std::string, std::function<Object*()> > class_map_;
};

std::unordered_map<std::string, std::function<Object*()> > ClassFactory::class_map_;

template<typename T>
class Register {
public:
    Register(const std::string& class_name) {
        ClassFactory::Get().RegisterClass(
            class_name, 
            [](){ return new T(); }
        );
    }

    // template<typename ...Args>
    // Register(const std::string& class_name, Args... args) {
    //     ClassFactory::Get().RegisterClass(
    //         class_name, 
    //         [&args](){ return new T(args); }
    //     );
    // }
};

#define REGISTER_CLASS(name, class_type, args...) static Register<class_type> register_##class_type (name, ##args);
