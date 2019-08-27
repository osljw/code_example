#include <iostream>
#include <string>
#include <unordered_map>
#include <functional>

class Resource {
public:
    virtual ~Resource() {}
    virtual void Load() = 0;
};

class ResourceManager {
public:
    typedef std::function<Resource*()> RegistrationFactory;

    static ResourceManager& GetRM() {
        static ResourceManager instance;
        return instance;
    }

    std::unordered_map<std::string, RegistrationFactory> path_monitor;

    void scan() {
        for(auto ele : path_monitor) {
            std::cout << "name:" << ele.first << std::endl;
            ele.second();
        }
    }
};

template <typename Factory>
class Register {
public:
    Register(const std::string& dir) {
        ResourceManager::GetRM().path_monitor.emplace(dir, [dir]{ return new Factory(dir); });
    }
};

#define REGISTER(dir, factory) REGISTER_UNIQ_HELPER(__COUNTER__, dir, factory)
#define REGISTER_UNIQ_HELPER(ctr, dir, factory) REGISTER_UNIQ(ctr, dir, factory)
#define REGISTER_UNIQ(ctr, dir, factory) \
    static Register<factory> register_##ctr = Register<factory>(dir);


class HelloWorld : public Resource {
public:
    HelloWorld(const std::string& _path) {
        path = _path;
        std::cout << "load path:" << path << std::endl;
    }

    void Load() {
        std::cout << "load path:" << path << std::endl;
    }

    virtual ~HelloWorld() {};

    std::string path;
};

REGISTER("test.txt", HelloWorld)
REGISTER("2test.txt", HelloWorld)

//Register<HelloWorld> r1("ttt"); 
