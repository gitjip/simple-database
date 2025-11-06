#pragma once
#include "Utils.h"

class Field
{
    friend class Record;
    friend std::ostream& operator<<(std::ostream& os, const Field& field);
    friend std::ofstream& operator<<(std::ofstream& ofs, const Field& field);
    friend std::ifstream& operator>>(std::ifstream& ifs, Field& field);
    friend std::istringstream& operator>>(std::istringstream& iss, Field& field);

public:
    Field(Utils::DataType dataType, void* data = nullptr);
    Field(const Field& field);
    ~Field();
    Field& operator=(const Field& source);
    bool operator==(const Field& compared);

private:
    void ReadAsInt(std::ifstream& ifs);
    void ReadAsBool(std::ifstream& ifs);
    void ReadAsFloat(std::ifstream& ifs);
    void ReadAsString(std::ifstream& ifs);

    void ReadAsInt(std::istringstream& iss);
    void ReadAsBool(std::istringstream& iss);
    void ReadAsFloat(std::istringstream& iss);
    void ReadAsString(std::istringstream& iss);

    static void SafeDelete(void*& block);

    Utils::DataType dataType;
    void* data;
};

std::ostream& operator<<(std::ostream& os, const Field& field);
std::ofstream& operator<<(std::ofstream& ofs, const Field& field);
std::ifstream& operator>>(std::ifstream& ifs, Field& field);
std::istringstream& operator>>(std::istringstream& iss, Field& field);