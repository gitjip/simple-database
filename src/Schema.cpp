#include <sstream>
#include <iomanip>
#include <string>
#include "Schema.h"
#include "Utils.h"

std::ostream& operator<<(std::ostream& os, const Schema& schema)
{
	for (const auto& field : schema.fields) {
		os << std::left << std::setw(Utils::COL_INTERVAL) << field.first + " " + Utils::ToName(field.second) << "|";
	}
	return os;
}

std::ofstream& operator<<(std::ofstream& ofs, const Schema& schema)
{
	for (const auto& field : schema.fields) {
		ofs << std::left << std::setw(Utils::COL_INTERVAL) << field.first + " " + Utils::ToName(field.second) << " ";
	}
	return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, Schema& schema)
{
	std::string line;
	std::getline(ifs, line);
	std::istringstream iss(line);

	std::string name;
	std::string dataType;
	while (iss >> name >> dataType) {
		schema.fields.emplace(name, Utils::ToDataType(dataType));
	}
	return ifs;
}