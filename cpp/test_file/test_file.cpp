#include <iostream>
#include <stdlib.h>

#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

void test_stat() {
    struct stat sbuf;
    int ret = stat("test.txt", &sbuf);
    if(ret !=0) std::cout << "test stat file not exist, ret:" << ret << std::endl;
    else std::cout<< "mtime:" << sbuf.st_mtime << std::endl;
}

int main() {
    test_stat();
}
