#pragma once
#include <iostream>
#include <fstream>
#include "Schema.h"

class Record
{
    friend class Table;
    friend std::ofstream& operator<<(std::ofstream& ofs, const Record& record);
    friend std::ifstream& operator>>(std::ifstream& ifs, Record& record);
    friend std::ostream& operator<<(std::ostream& ofs, const Record& record);

public:
    Record();
    Record(const Record& record);
    Record(const Schema& schema);
    ~Record();
    Record& operator=(const Schema& schema);

private:
    std::map<std::string, Field*> fields;
};

std::ofstream& operator<<(std::ofstream& ofs, const Record& record);
std::ifstream& operator>>(std::ifstream& ifs, Record& record);
std::ostream& operator<<(std::ostream& os, const Record& record);