#include <iostream>
#include <sstream>
#include <filesystem>
#include "Database.h"
#include "Exception.h"

Database::Database(const std::filesystem::path& path, const std::string& name) :
	Entity(path), name(name)
{
	std::filesystem::create_directory(path);
}

Database::~Database()
{
	for (const auto& tbl : tables) {
		delete tbl.second;
	}
}

Utils::EntityType Database::GetType() const
{
	return Utils::EntityType::DATABASE;
}

std::string Database::GetName() const
{
	return name;
}

Entity* Database::ParseCommand(const std::string& cmdLine)
{
	std::istringstream iss(cmdLine);
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd == "USE") {
		Entity* usedTable = ParseUse(iss);
		return usedTable;
	} else if (cmd == "CREATE") {
		ParseInsert(iss);
	} else if (cmd == "DROP") {
		ParseRemove(iss);
	} else if (cmd == "ALTER") {
		ParseUpdate(iss);
	} else if (cmd == "SELECT") {
		ParseSelect(iss);
	} else {
		throw Exception("Unknown command " + cmd);
	}
	return nullptr;
}

Entity* Database::ParseUse(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "TABLE") {
		throw Exception("Expected TABLE");
	}
	std::string tblName;
	iss >> tblName;
	if (tblName.empty()) {
		throw Exception("Expected table name");
	}
	auto foundTbl = tables.find(tblName);
	if (foundTbl == tables.end()) {
		throw Exception("Table " + tblName + " not found");
	}
	return foundTbl->second;
}

void Database::ParseInsert(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "TABLE") {
		throw Exception("Expected TABLE");
	}
	std::string tblName;
	iss >> tblName;
	if (tblName.empty()) {
		throw Exception("Expected table name");
	}
	auto foundTbl = tables.find(tblName);
	if (foundTbl != tables.end()) {
		throw Exception("Table " + tblName + " already exists");
	}
	Schema schema = Table::ParseSchema(iss);
	InsertTable(tblName, schema);
}

void Database::ParseRemove(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "TABLE") {
		throw Exception("Expected TABLE");
	}
	std::string tblName;
	iss >> tblName;
	if (tblName.empty()) {
		throw Exception("Expected table name");
	}
	auto foundTbl = tables.find(tblName);
	if (foundTbl == tables.end()) {
		throw Exception("Table " + tblName + " not found");
	}
	RemoveTable(tblName);
}

void Database::ParseUpdate(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "TABLE") {
		throw Exception("Expected TABLE");
	}

	std::string oldTblName;
	iss >> oldTblName;
	if (oldTblName.empty()) {
		throw Exception("Expected old table name");
	}
	auto foundTarget = tables.find(oldTblName);
	if (foundTarget == tables.end()) {
		throw Exception("Table " + oldTblName + " not found");
	}

	std::string newTblName;
	iss >> newTblName;
	if (newTblName.empty()) {
		throw Exception("Expected new table name");
	}
	auto foundExisting = tables.find(newTblName);
	if (foundExisting != tables.end()) {
		throw Exception("Table " + newTblName + " already exists");
	}

	UpdateTable(oldTblName, newTblName);
}

void Database::ParseSelect(std::istringstream& iss)
{
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd != "TABLE") {
		throw Exception("Expected TABLE");
	}
	SelectTable();
}

void Database::LoadFromFile()
{
	for (auto& table : tables) {
		delete table.second;
	}
	tables.clear();
	for (const auto& entry : std::filesystem::directory_iterator(storagePath)) {
		std::string tblName = entry.path().stem().string();
		tables.emplace(tblName, new Table(ToPath(tblName, Table::extension), tblName));
	}
}

void Database::InsertTable(const std::string& tblName, const Schema& schema)
{
	tables.emplace(tblName, new Table(ToPath(tblName, Table::extension), tblName, schema));
	std::cout << "Table " + tblName + " created" << std::endl;
}

void Database::RemoveTable(const std::string& tblName)
{
	std::filesystem::remove(ToPath(tblName, Table::extension));
	delete tables[tblName];
	tables.erase(tblName);
	std::cout << "Table " + tblName + " dropped" << std::endl;
}

void Database::UpdateTable(const std::string& oldTblName, const std::string& newTblName)
{
	std::filesystem::rename(ToPath(oldTblName, Table::extension), ToPath(newTblName, Table::extension));
	tables[oldTblName]->name = newTblName;
	auto copy = tables[oldTblName];
	tables.erase(oldTblName);
	tables.emplace(newTblName, copy);
	std::cout << "Table " + oldTblName + " renamed as " + newTblName << std::endl;
}

void Database::SelectTable()
{
	int count = 0;
	std::cout << std::endl;
	std::cout << "TABLE VIEW: " << std::endl;
	std::cout << std::endl;
	for (const auto& tbl : tables) {
		std::cout << std::right << std::setw(6) << ++count << " >> " << tbl.second->GetName() << std::endl;
	}
	if (count == 0) {
		std::cout << "       >> " << "No table yet" << std::endl;
	}
	std::cout << std::endl;
}