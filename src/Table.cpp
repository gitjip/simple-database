#include <iostream>
#include <sstream>
#include <fstream>
#include <string>
#include "Table.h"
#include "Exception.h"

const std::string Table::extension = "tbl";

Table::Table(const std::filesystem::path& path, const std::string& name, const Schema& schema) :
    Entity(path), name(name), schema(schema)
{
    if (!std::filesystem::exists(path)) {
        SaveToFile();
    } else {
        LoadFromFile();
    }
}

Table::~Table()
{
    for (auto record : records) {
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
    if (cmd == "USE") {
        throw Exception("Cannot use from table");
    } else if (cmd == "INSERT") {
        ParseInsert(iss);
    } else if (cmd == "DELETE") {
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
    throw Exception("Cannot use from table");
}

void Table::ParseInsert(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "INTO") {
        throw Exception("Expected INTO");
    }
    std::string tblName;
    iss >> tblName;
    if (tblName != name) {
        throw Exception("Table name mismatch");
    }
    Record record = ParseRecord(iss);
    InsertRecord(record);
}

void Table::ParseRemove(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "FROM") {
        throw Exception("Expected FROM");
    }
    std::string tblName;
    iss >> tblName;
    if (tblName != name) {
        throw Exception("Table name mismatch");
    }
    std::string fieldName;
    iss >> fieldName;
    if (fieldName.empty()) {
        throw Exception("Expected field name");
    }
    auto foundField = schema.fields.find(fieldName);
    if (foundField == schema.fields.end()) {
        throw Exception("Field " + fieldName + " not found");
    }
    Field compared(foundField->second);
    iss >> compared;
    RemoveRecord(fieldName, compared);
}

void Table::ParseUpdate(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "FROM") {
        throw Exception("Expected FROM");
    }
    std::string tblName;
    iss >> tblName;
    if (tblName != name) {
        throw Exception("Table name mismatch");
    }
    std::string mdfName;
    iss >> mdfName;
    if (mdfName.empty()) {
        throw Exception("Expected modified field name");
    }
    auto foundMdfField = schema.fields.find(mdfName);
    if (foundMdfField == schema.fields.end()) {
        throw Exception("Field " + mdfName + " not found");
    }
    Field modified(foundMdfField->second);
    iss >> modified;
    std::string cmpName;
    iss >> cmpName;
    if (cmpName.empty()) {
        throw Exception("Expected compared field name");
    }
    auto foundCmpField = schema.fields.find(cmpName);
    if (foundCmpField == schema.fields.end()) {
        throw Exception("Field " + cmpName + " not found");
    }
    Field compared(foundCmpField->second);
    iss >> compared;
    UpdateRecord(mdfName, modified, cmpName, compared);
}

void Table::ParseSelect(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "FROM") {
        throw Exception("Expected FROM");
    }
    std::string tblName;
    iss >> tblName;
    if (tblName != name) {
        throw Exception("Table name mismatch");
    }
    std::string fieldName;
    iss >> fieldName;
    if (fieldName.empty()) {
        SelectAllRecords();
    } else {
        auto foundField = schema.fields.find(fieldName);
        if (foundField == schema.fields.end()) {
            throw Exception("Field " + fieldName + " not found");
        }
        Field compared(foundField->second);
        iss >> compared;
        SelectRecord(fieldName, compared);
    }
}

void Table::LoadFromFile()
{
    std::ifstream ifs(storagePath);
    if (!ifs.is_open()) {
        throw Exception("Failed to open file " + storagePath.string());
    }
    LoadSchema(ifs);
    LoadRecords(ifs);
    ifs.close();
}

void Table::InsertRecord(const Record& record)
{
    records.push_back(new Record(record));
    SaveToFile();
    std::cout << "Record inserted" << std::endl;
}

void Table::RemoveRecord(const std::string& fieldName, const Field& compared)
{
    int count = 0;
    for (auto it = records.begin(); it != records.end();) {
        auto foundField = (*it)->fields.find(fieldName);
        if (foundField != (*it)->fields.end() && *foundField->second == compared) {
            delete *it;
            it = records.erase(it);
            count++;
        } else {
            it++;
        }
    }
    SaveToFile();
    std::cout << count << " record(s) deleted" << std::endl;
}

void Table::UpdateRecord(const std::string& mdfName, const Field& modified, const std::string& cmpName, const Field& compared)
{
    int count = 0;
    for (auto record : records) {
        auto foundCmpField = record->fields.find(cmpName);
        if (foundCmpField != record->fields.end() && *foundCmpField->second == compared) {
            auto foundMdfField = record->fields.find(mdfName);
            if (foundMdfField != record->fields.end()) {
                *foundMdfField->second = modified;
                count++;
            }
        }
    }
    SaveToFile();
    std::cout << count << " record(s) updated" << std::endl;
}

void Table::SelectRecord(const std::string& fieldName, const Field& compared)
{
    int count = 0;
    PrintDividerLine();
    std::cout << schema << std::endl;
    PrintDividerLine();
    for (auto record : records) {
        auto foundField = record->fields.find(fieldName);
        if (foundField != record->fields.end() && *foundField->second == compared) {
            std::cout << *record << std::endl;
            count++;
        }
    }
    PrintDividerLine();
    std::cout << count << " record(s) selected" << std::endl;
}

void Table::SelectAllRecords()
{
    PrintDividerLine();
    std::cout << schema << std::endl;
    PrintDividerLine();
    for (auto record : records) {
        std::cout << *record << std::endl;
    }
    PrintDividerLine();
    std::cout << records.size() << " record(s) selected" << std::endl;
}

Schema Table::ParseSchema(std::istringstream& iss)
{
    Schema schema;
    std::string fieldName;
    while (iss >> fieldName) {
        if (fieldName == "(") {
            continue;
        } else if (fieldName == ")") {
            break;
        }
        std::string dataTypeName;
        iss >> dataTypeName;
        if (dataTypeName.empty()) {
            throw Exception("Expected data type for field " + fieldName);
        }
        schema.fields.emplace(fieldName, Utils::ToDataType(dataTypeName));
    }
    return schema;
}

Record Table::ParseRecord(std::istringstream& iss) const
{
    Record record(schema);
    for (auto& field : record.fields) {
        iss >> *field.second;
    }
    return record;
}

void Table::SaveToFile()
{
    std::ofstream ofs(storagePath);
    if (!ofs.is_open()) {
        throw Exception("Failed to open file " + storagePath.string());
    }
    SaveSchema(ofs);
    SaveRecords(ofs);
    ofs.close();
}

void Table::SaveSchema(std::ofstream& ofs) const
{
    ofs << schema;
}

void Table::SaveRecords(std::ofstream& ofs)
{
    for (auto record : records) {
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
    for (int i = 0; i < schema.fields.size(); i++) {
        for (int j = 0; j < Utils::COL_INTERVAL; j++) {
            std::cout << "-";
        }
        std::cout << "+";
    }
    std::cout << std::endl;
}