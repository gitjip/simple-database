#include <sstream>
#include <iomanip>
#include <string>
#include "Schema.h"
#include "Utils.h"

std::ostream& operator<<(std::ostream& os, const Schema& schema)
{
    for (const auto& field : schema.fields) {
        os << std::left << std::setw(Utils::COL_INTERVAL) << field.first;
    }
    return os;
}

std::ofstream& operator<<(std::ofstream& ofs, const Schema& schema)
{
    for (const auto& field : schema.fields) {
        ofs << field.first << " " << Utils::ToName(field.second) << std::endl;
    }
    return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, Schema& schema)
{
    std::string line;
    while (std::getline(ifs, line)) {
        if (line.empty()) {
            break;
        }
        std::istringstream iss(line);
        std::string fieldName;
        std::string dataTypeName;
        iss >> fieldName >> dataTypeName;
        if (fieldName.empty() || dataTypeName.empty()) {
            throw Exception("Invalid schema format");
        }
        schema.fields.emplace(fieldName, Utils::ToDataType(dataTypeName));
    }
    return ifs;
}