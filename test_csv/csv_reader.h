#ifndef CSV_READER_H
#define CSV_READER_H

#include <istream>
#include <iostream>
#include <string>
#include <vector>

enum class CSVState {
    UnquotedField,
    QuotedField,
    QuotedQuote
};

std::vector<std::string> ReadCSVRow(const std::string &row);
std::vector<std::vector<std::string>> readCSV(std::istream &in);

#endif /* CSV_READER_H */
