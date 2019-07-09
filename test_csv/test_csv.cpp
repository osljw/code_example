#include <istream>
#include <iostream>
#include <fstream>
#include <string>
#include <vector>

#include "csv_reader.h"

int main() {
    std::ifstream in_file;
    in_file.open("input/test.csv", std::ifstream::in);
    std::vector<std::vector<std::string>> content;

    if(in_file)
        content = readCSV(in_file);

    for(auto v : content) {
        std::cout << "============ len:" << v.size() << std::endl;
        for(auto s: v) {
            std::cout << s << std::endl;
        }
    }

    return 0;
}

