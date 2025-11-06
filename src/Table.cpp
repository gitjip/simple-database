#include <iostream>
#include <sstream>
#include <fstream>
#include <iomanip>
#include <filesystem>
#include "Table.h"
#include "Exception.h"
#include "Utils.h"

Table::Table(const std::filesystem::path& path, const std::string& name, const Schema& schema) :
	Entity(path), name(name), schema(schema)
{
	if (!std::filesystem::exists(path)) {
		SaveToFile();
	}
}

Table::~Table()
{
	for (const auto& record : records) {
		delete record;
	}
}

Utils::EntityType Table::GetType() const
{
	return Utils::EntityType::TABLE;
}

std::string Table::GetName() const
{
	return name;
}

Entity* Table::ParseCommand(const std::string& cmdLine)
{
	std::istringstream iss(cmdLine);
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd == "INSERT") {
		ParseInsert(iss);
	} else if (cmd == "REMOVE") {
		ParseRemove(iss);
	} else if (cmd == "UPDATE") {
		ParseUpdate(iss);
	} else if (cmd == "SELECT") {
		ParseSelect(iss);
	} else {
		throw Exception("Unknown command " + cmd);
	}
	return nullptr;
}

Entity* Table::ParseUse(std::istringstream& iss)
{
	return nullptr;
}

void Table::ParseInsert(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "RECORD") {
		throw Exception("Expected RECORD");
	}
	InsertRecord(ParseRecord(iss));
	SaveToFile();
	std::cout << "Record inserted successfully" << std::endl;
}

void Table::ParseRemove(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "RECORD") {
		throw Exception("Expected RECORD");
	}
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "WHERE") {
		throw Exception("Expected WHERE");
	}
	std::string fieldName;
	iss >> fieldName;
	auto foundField = schema.fields.find(fieldName);
	if (foundField == schema.fields.end()) {
		throw Exception("Field name not found");
	}
	Field field(foundField->second);
	iss >> field;
	RemoveRecord(fieldName, field);
	SaveToFile();
	std::cout << "Records removed successfully" << std::endl;
}

void Table::ParseUpdate(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "RECORD") {
		throw Exception("Expected RECORD");
	}

	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "SET") {
		throw Exception("Expected SET");
	}
	std::string modifiedFieldName;
	iss >> modifiedFieldName;
	auto foundModifiedField = schema.fields.find(modifiedFieldName);
	if (foundModifiedField == schema.fields.end()) {
		throw Exception("Field " + modifiedFieldName + " to modify not found");
	}
	Field modifiedField(foundModifiedField->second);
	iss >> modifiedField;

	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "WHERE") {
		throw Exception("Expected WHERE");
	}
	std::string comparedFieldName;
	iss >> comparedFieldName;
	auto foundComparedField = schema.fields.find(comparedFieldName);
	if (foundComparedField == schema.fields.end()) {
		throw Exception("Field" + comparedFieldName + " to compare not found");
	}
	Field comparedField(foundComparedField->second);
	iss >> comparedField;

	UpdateRecord(modifiedFieldName, modifiedField, comparedFieldName, comparedField);
	SaveToFile();
	std::cout << "Records modified successfully" << std::endl;
}

void Table::ParseSelect(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "RECORD") {
		throw Exception("Expected RECORD");
	}
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd == "*") {
		SelectAllRecords();
		return;
	}
	if (cmd != "WHERE") {
		throw Exception("Expected WHERE");
	}
	std::string fieldName;
	iss >> fieldName;
	auto foundField = schema.fields.find(fieldName);
	if (foundField == schema.fields.end()) {
		throw Exception("Field name not found");
	}
	Field field(foundField->second);
	iss >> field;
	SelectRecord(fieldName, field);
}

void Table::LoadFromFile()
{
	std::ifstream ifs(storagePath, std::ios::in);
	if (!ifs.is_open()) {
		throw Exception("Cannot open file " + storagePath.string());
	}
	LoadSchema(ifs);
	LoadRecords(ifs);
}

void Table::InsertRecord(const Record& record)
{
	records.emplace_back(new Record(record));
}

void Table::RemoveRecord(const std::string& fieldName, const Field& compared)
{
	records.remove_if([fieldName, compared](Record* record) {
		return *record->fields[fieldName] == compared;
		});
}

void Table::UpdateRecord(const std::string& mdfName, const Field& modified, const std::string& cmpName, const Field& compared)
{
	for (const auto& record : records) {
		if (*record->fields[cmpName] == compared) {
			*record->fields[mdfName] = modified;
		}
	}
}

void Table::SelectRecord(const std::string& fieldName, const Field& compared)
{
	int count = 0;
	std::cout << std::endl;
	std::cout << "RECORDS SELECTED: " << std::endl;
	std::cout << std::endl;
	PrintDividerLine();
	std::cout << "       >> " << schema << std::endl;
	PrintDividerLine();
	for (const auto& record : records) {
		if (*record->fields[fieldName] == compared) {
			std::cout << std::right << std::setw(6) << ++count << " >> " << *record << std::endl;
		}
	}
	if (count == 0) {
		std::cout << "       >> " << "No records yet" << std::endl;
	}
	PrintDividerLine();
	std::cout << std::endl;
}

void Table::SelectAllRecords()
{
	int count = 0;
	std::cout << std::endl;
	std::cout << "TABLE VIEW: " << std::endl;
	std::cout << std::endl;
	PrintDividerLine();
	std::cout << "       >> " << schema << std::endl;
	PrintDividerLine();
	for (const auto& record : records) {
		std::cout << std::right << std::setw(6) << ++count << " >> " << *record << std::endl;
	}
	if (count == 0) {
		std::cout << "       >> " << "No record yet" << std::endl;
	}
	PrintDividerLine();
	std::cout << std::endl;
}

Schema Table::ParseSchema(std::istringstream& iss)
{
	Schema schema;
	std::string fieldName, dataType;
	while (iss >> fieldName) {
		iss >> dataType;
		if (dataType.empty()) {
			throw Exception("Expected data type");
		}
		schema.fields.emplace(fieldName, Utils::ToDataType(dataType));
	}
	if (schema.fields.empty()) {
		throw Exception("Schema empty");
	}
	return schema;
}

Record Table::ParseRecord(std::istringstream& iss) const
{
	Record ret = schema;
	for (auto& field : ret.fields) {
		iss >> *(field.second);
	}
	return ret;
}

void Table::SaveToFile()
{
	std::ofstream ofs(storagePath, std::ios::out);
	if (!ofs.is_open()) {
		throw Exception("Cannot open file " + storagePath.string());
	}
	SaveSchema(ofs);
	SaveRecords(ofs);
}

void Table::SaveSchema(std::ofstream& ofs) const
{
	ofs << schema << std::endl;
}

void Table::SaveRecords(std::ofstream& ofs)
{
	for (const auto& record : records) {
		ofs << *record << std::endl;
	}
}

void Table::LoadSchema(std::ifstream& ifs)
{
	ifs >> schema;
}

void Table::LoadRecords(std::ifstream& ifs)
{
	Record record;
	while (ifs >> (record = schema)) {
		records.emplace_back(new Record(record));
	}
}

void Table::PrintDividerLine() const
{
	std::cout << "       >> " << std::setfill('-')
		<< std::setw((static_cast<unsigned long long>(Utils::COL_INTERVAL) + 1) * (schema.fields.size()))
		<< "" << std::setfill(' ') << std::endl;
}

std::string const Table::extension = "txt";