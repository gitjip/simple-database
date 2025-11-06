#include <iostream>
#include <sstream>
#include <string>
#include "Record.h"
#include "Utils.h"

Record::Record()
{
}

Record::Record(const Record& record)
{
	for (const auto& field : record.fields) {
		fields.emplace(field.first, new Field(*field.second));
	}
}

Record::Record(const Schema& schema)
{
	for (const auto& field : schema.fields) {
		fields.emplace(field.first, new Field(field.second));
	}
}

Record::~Record()
{
	for (const auto& field : fields) {
		delete field.second;
	}
}

Record& Record::operator=(const Schema& schema)
{
	for (const auto& field : fields) {
		delete field.second;
	}
	fields.clear();
	for (const auto& field : schema.fields) {
		fields.emplace(field.first, new Field(field.second));
	}
	return *this;
}

std::ofstream& operator<<(std::ofstream& ofs, const Record& record)
{
	for (const auto& field : record.fields) {
		ofs << *(field.second) << " ";
	}
	return ofs;
}

std::ifstream& operator>>(std::ifstream& ifs, Record& record)
{
	std::string line;
	std::getline(ifs, line);
	if (line.empty()) {
		return ifs;
	}
	std::istringstream iss(line);

	for (auto& field : record.fields) {
		iss >> *(field.second);
	}
	return ifs;
}

std::ostream& operator<<(std::ostream& os, const Record& record)
{
	for (const auto& field : record.fields) {
		os << *(field.second) << "|";
	}
	return os;
}