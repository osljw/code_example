#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include <iostream>

#include "hdfs.h"

int main(int argc, char **argv) {

    // default 表示从本地主机
    hdfsFS fs = hdfsConnect("default", 0);
    if(!fs) {
        std::cout << "hdfsConnect error" << std::endl;
        return -1;
    }

    const char* filePath = "/tmp/tmp_testfile.txt";

    //hdfsFile writeFile = hdfsOpenFile(fs, filePath, O_WRONLY | O_CREAT, 0, 0, 0);
    //if(!writeFile) {
    //      fprintf(stderr, "Failed to open %s for writing!\n", filePath);
    //      exit(-1);
    //}

    //char* buffer = "Hello, World!";
    //tSize num_written_bytes = hdfsWrite(fs, writeFile, (void*)buffer, strlen(buffer)+1);
    //if (hdfsFlush(fs, writeFile)) {
    //       fprintf(stderr, "Failed to 'flush' %s\n", filePath);
    //      exit(-1);
    //}
    //std::cout << "hdfsWrite:" << buffer << std::endl;
    //hdfsCloseFile(fs, writeFile);

    ///////////////////////

    hdfsFile readFile = hdfsOpenFile(fs, filePath, O_RDONLY, 0, 0, 0);
    if(!readFile) {
          std::cout << "read hdfsOpenFile error" << std::endl;
          return -1;
    }

    char* read_buffer = new char[4096];
    int bufferSize = 5;
    tSize result = hdfsRead(fs, readFile, (void*)read_buffer, bufferSize);
    if(result < 0) {
        std::cout << "hdfsRead fail" << std::endl;
        return -1;
    }
    std::cout << "read data:" << read_buffer << std::endl;

    hdfsCloseFile(fs, readFile);
}
