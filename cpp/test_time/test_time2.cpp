#include <time.h>
#include <iostream>


int main() {

    time_t local_time = time(NULL);
    struct tm stime;   //tm结构指针
    localtime_r(&local_time, &stime);   //获取当地日期和时间
    std::cout << "now time stamp: " << local_time << std::endl;
    std::cout << "hour: " << stime.tm_hour << std::endl;
    std::cout << "min: " << stime.tm_min << std::endl;
}
