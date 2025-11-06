#include <algorithm>
#include "Utils.h"

std::string Utils::ToName(EntityType et)
{
	if (et == EntityType::MANAGER) {
		return "Manager";
	} else if (et == EntityType::DATABASE) {
		return "Database";
	} else if (et == EntityType::TABLE) {
		return "Table";
	} else if (et == EntityType::RECORD) {
		return "Record";
	} else if (et == EntityType::FIELD) {
		return "Field";
	} else {
		throw Exception("Invalid entity type");
	}
}

std::string Utils::ToName(DataType dt)
{
	if (dt == DataType::INT) {
		return "INT";
	} else if (dt == DataType::BOOL) {
		return "BOOL";
	} else if (dt == DataType::FLOAT) {
		return "FLOAT";
	} else if (dt == DataType::STRING) {
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