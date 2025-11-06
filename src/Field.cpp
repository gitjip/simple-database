#include <iomanip>
#include <fstream>
#include <sstream>
#include "Field.h"

Field::Field(Utils::DataType dataType, void* data) :
    dataType(dataType)
{
    if (data == nullptr) {
        return;
    }
    if (dataType == Utils::DataType::INT) {
        this->data = new int(*static_cast<int*>(data));
    } else if (dataType == Utils::DataType::BOOL) {
        this->data = new bool(*static_cast<bool*>(data));
    } else if (dataType == Utils::DataType::FLOAT) {
        this->data = new double(*static_cast<double*>(data));
    } else if (dataType == Utils::DataType::STRING) {
        this->data = new std::string(*static_cast<std::string*>(data));
    }
}

Field::Field(const Field& field) :
    Field(field.dataType, field.data)
{
}

Field::~Field()
{
    SafeDelete(data);
}

Field& Field::operator=(const Field& src)
{
    SafeDelete(data);
    if (src.data == nullptr) {
        return *this;
    }
    if (dataType == Utils::DataType::INT) {
        data = new int(*static_cast<int*>(src.data));
    } else if (dataType == Utils::DataType::BOOL) {
        data = new bool(*static_cast<bool*>(src.data));
    } else if (dataType == Utils::DataType::FLOAT) {
        data = new double(*static_cast<double*>(src.data));
    } else if (dataType == Utils::DataType::STRING) {
        data = new std::string(*static_cast<std::string*>(src.data));
    }
    return *this;
}

bool Field::operator==(const Field& compared)
{
    if (data == nullptr || compared.data == nullptr) {
        throw Exception("data empty when comparing two fields");
    }
    if (dataType == Utils::DataType::INT) {
        return *static_cast<int*>(data) == *static_cast<int*>(compared.data);
    } else if (dataType == Utils::DataType::BOOL) {
        return *static_cast<bool*>(data) == *static_cast<bool*>(compared.data);
    } else if (dataType == Utils::DataType::FLOAT) {
        return *static_cast<double*>(data) == *static_cast<double*>(compared.data);
    } else if (dataType == Utils::DataType::STRING) {
        return *static_cast<std::string*>(data) == *static_cast<std::string*>(compared.data);
    } else {
        throw Exception("Invalid data type when comparing two fields");
    }
}

void Field::ReadAsInt(std::ifstream& ifs)
{
    int val;
    if (ifs >> val) {
        SafeDelete(data);
        data = new int(val);
    } else {
        throw Exception("Invalid int value when reading fields");
    }
}

void Field::ReadAsBool(std::ifstream& ifs)
{
    std::string val;
    if (ifs >> val) {
        val = Utils::ToUpperString(val);
        if (val == "TRUE") {
            SafeDelete(data);
            data = new bool(true);
        } else if (val == "FALSE") {
            SafeDelete(data);
            data = new bool(false);
        } else {
            throw Exception("Invalid bool value when reading fields");
        }
    } else {
        throw Exception("Invalid bool value when reading fields");
    }
}

void Field::ReadAsFloat(std::ifstream& ifs)
{
    double val;
    if (ifs >> val) {
        SafeDelete(data);
        data = new double(val);
    } else {
        throw Exception("Invalid double value when reading fields");
    }
}

void Field::ReadAsString(std::ifstream& ifs)
{
    std::string val;
    if (ifs >> val) {
        SafeDelete(data);
        data = new std::string(val);
    } else {
        throw Exception("Invalid string value when reading fields");
    }
}

void Field::ReadAsInt(std::istringstream& iss)
{
    int val;
    if (iss >> val) {
        SafeDelete(data);
        data = new int(val);
    } else {
        throw Exception("Invalid int value when parsing fields");
    }
}

void Field::ReadAsBool(std::istringstream& iss)
{
    std::string val;
    if (iss >> val) {
        val = Utils::ToUpperString(val);
        if (val == "TRUE") {
            SafeDelete(data);
            data = new bool(true);
        } else if (val == "FALSE") {
            SafeDelete(data);
            data = new bool(false);
        } else {
            throw Exception("Invalid bool value when parsing fields");
        }
    } else {
        throw Exception("Invalid bool value when parsing fields");
    }
}

void Field::ReadAsFloat(std::istringstream& iss)
{
    double val;
    if (iss >> val) {
        SafeDelete(data);
        data = new double(val);
    } else {
        throw Exception("Invalid double value when parsing fields");
    }
}

void Field::ReadAsString(std::istringstream& iss)
{
    std::string val;
    if (iss >> val) {
        SafeDelete(data);
        data = new std::string(val);
    } else {
        throw Exception("Invalid string value when parsing fields");
    }
}

void Field::SafeDelete(void*& block)
{
    if (block != nullptr) {
        delete block;
        block = nullptr;
    }
}

std::ostream& operator<<(std::ostream& os, const Field& field)
{
    if (field.data == nullptr) {
        throw Exception("Data empty when printing field");
    }
    if (field.dataType == Utils::DataType::INT) {
        os << std::left << std::setw(Utils::COL_INTERVAL) << *static_cast<int*>(field.data);
    } else if (field.dataType == Utils::DataType::BOOL) {
        os << std::left << std::setw(Utils::COL_INTERVAL) << (*static_cast<bool*>(field.data) ? "TRUE" : "FALSE");
    } else if (field.dataType == Utils::DataType::FLOAT) {
        os << std::left << std::setw(Utils::COL_INTERVAL) << *static_cast<double*>(field.data);
    } else if (field.dataType == Utils::DataType::STRING) {
        os << std::left << std::setw(Utils::COL_INTERVAL) << *static_cast<std::string*>(field.data);
    } else {
        throw Exception("Invalid datatype of field");
    }
    return os;
}

std::ofstream& operator<<(std::ofstream& ofs, const Field& field)
{
    if (field.data == nullptr) {
        throw Exception("Data empty when writing field");
    }
    if (field.dataType == Utils::DataType::INT) {
        ofs << std::left << std::setw(Utils::COL_INTERVAL) << *static_cast<int*>(field.data);
    } else if (field.dataType == Utils::DataType::BOOL) {
        ofs << std::left << std::setw(Utils::COL_INTERVAL) << (*static_cast<bool*>(field.data) ? "TRUE" : "FALSE");
    } else if (field.dataType == Utils::DataType::FLOAT) {
        ofs << std::left << std::setw(Utils::COL_INTERVAL) << *static_cast<double*>(field.data);
    } else if (field.dataType == Utils::DataType::STRING) {
        ofs << std::left << std::setw(Utils::COL_INTERVAL) << *static_cast<std::string*>(field.data);
    } else {
        throw Exception("Invalid datatype of field");
    }
    return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, Field& field)
{
    if (field.dataType == Utils::DataType::INT) {
        field.ReadAsInt(ifs);
    } else if (field.dataType == Utils::DataType::BOOL) {
        field.ReadAsBool(ifs);
    } else if (field.dataType == Utils::DataType::FLOAT) {
        field.ReadAsFloat(ifs);
    } else if (field.dataType == Utils::DataType::STRING) {
        field.ReadAsString(ifs);
    } else {
        throw Exception("Invalid datatype of field");
    }
    return ifs;
}

std::istringstream& operator>>(std::istringstream& iss, Field& field)
{
    if (field.dataType == Utils::DataType::INT) {
        field.ReadAsInt(iss);
    } else if (field.dataType == Utils::DataType::BOOL) {
        field.ReadAsBool(iss);
    } else if (field.dataType == Utils::DataType::FLOAT) {
        field.ReadAsFloat(iss);
    } else if (field.dataType == Utils::DataType::STRING) {
        field.ReadAsString(iss);
    } else {
        throw Exception("Invalid datatype of field");
    }
    return iss;
}