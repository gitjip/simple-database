#include <algorithm>
#include "Utils.h"

std::string Utils::ToName(EntityType entityType)
{
    if (entityType == EntityType::MANAGER) {
        return "MANAGER";
    } else if (entityType == EntityType::DATABASE) {
        return "DATABASE";
    } else if (entityType == EntityType::TABLE) {
        return "TABLE";
    } else if (entityType == EntityType::RECORD) {
        return "RECORD";
    } else if (entityType == EntityType::FIELD) {
        return "FIELD";
    } else {
        throw Exception("Invalid entity type");
    }
}

std::string Utils::ToName(DataType dataType)
{
    if (dataType == DataType::INT) {
        return "INT";
    } else if (dataType == DataType::BOOL) {
        return "BOOL";
    } else if (dataType == DataType::FLOAT) {
        return "FLOAT";
    } else if (dataType == DataType::STRING) {
        return "STRING";
    } else {
        throw Exception("Invalid data type");
    }
}

Utils::DataType Utils::ToDataType(std::string name)
{
	name = ToUpperString(name);
	if (name == "INT") {
		return DataType::INT;
	} else if (name == "BOOL") {
		return DataType::BOOL;
	} else if (name == "FLOAT") {
		return DataType::FLOAT;
	} else if (name == "STRING") {
		return DataType::STRING;
	} else {
		throw Exception("Invalid data type");
	}
}

std::string Utils::ToUpperString(std::string str)
{
    std::transform(str.begin(), str.end(), str.begin(), ::toupper);
    return str;
}