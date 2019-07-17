#include "tbb/concurrent_hash_map.h"

#include <iostream>
#include <functional>
#include <string>
#include <thread>

struct IntHashCompare {
  static std::hash<int64_t> hash_func;

  static size_t hash(const int64_t x) {
    return hash_func(x);
  }

  static bool equal(const int64_t x, const int64_t y) {
    return x==y;
  }
};

std::hash<int64_t> IntHashCompare::hash_func;


typedef tbb::concurrent_hash_map<int64_t, std::string, IntHashCompare> Table;


void test_w() {
    Table t;
    Table::accessor a;
    t.insert(a, 123);
    a->second = "hello tbb";
    //a.release();
    
    {
        Table::accessor a;
        //t.insert(a, 321); // will not block
        t.insert(a, 123); // if no a.release() will block
        a->second = "hello tbb 321";
        //a.release();
    }

}

void test_wr() {
    Table t;
    Table::accessor a;
    t.insert(a, 123);
    a->second = "hello tbb";
    //a.release();

    {
        Table::const_accessor ca;
        bool is_find = t.find(ca, 234); // if no a.release(), will block here
        std::cout << "key=234 find:" << is_find << std::endl;
    }
    
    Table::const_accessor ca;
    t.find(ca, 123); // if no a.release(), will block here
    std::cout << "key:123" << "value:" << ca->second << std::endl;
}

void test_error() {
    Table* pt;
    {
        Table t;
        pt = &t;

        Table::accessor a;
        t.insert(a, 123);
        a->second = "hello tbb";
    }
    //pt = nullptr;

    Table::const_accessor ca;
    bool is_find = pt->find(ca, 123); // if no a.release(), will block here
    std::cout <<"find:"<<is_find<<std::endl;
    std::cout << "key:123" << "value:" << ca->second << std::endl;
}

int main() {
    test_error();
}
