#include <iostream>
#include <sstream>
#include <string>
#include "Manager.h"
#include "Exception.h"

Manager::Manager(const std::filesystem::path& path) :
    Entity(path)
{
    std::filesystem::create_directories(path);
}

Manager::~Manager()
{
    for (const auto& db : databases) {
        delete db.second;
    }
}

Utils::EntityType Manager::GetType() const
{
    return Utils::EntityType::MANAGER;
}

std::string Manager::GetName() const
{
    return std::string();
}

Entity* Manager::ParseCommand(const std::string& cmdLine)
{
    std::istringstream iss(cmdLine);
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd == "USE") {
        Entity* usedDatabase = ParseUse(iss);
        return usedDatabase;
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

Entity* Manager::ParseUse(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "DATABASE") {
        throw Exception("Expected DATABASE");
    }
    std::string dbName;
    iss >> dbName;
    if (dbName.empty()) {
        throw Exception("Expected database name");
    }
    auto foundDb = databases.find(dbName);
    if (foundDb == databases.end()) {
        throw Exception("Database " + dbName + " not found");
    }
    return foundDb->second;
}

void Manager::ParseInsert(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "DATABASE") {
        throw Exception("Expected DATABASE");
    }
    std::string dbName;
    iss >> dbName;
    if (dbName.empty()) {
        throw Exception("Expected database name");
    }
    auto foundDb = databases.find(dbName);
    if (foundDb != databases.end()) {
        throw Exception("Database " + dbName + " already exists");
    }
    InsertDatabase(dbName);
}

void Manager::ParseRemove(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "DATABASE") {
        throw Exception("Expected DATABASE");
    }
    std::string dbName;
    iss >> dbName;
    if (dbName.empty()) {
        throw Exception("Expected database name");
    }
    auto foundDb = databases.find(dbName);
    if (foundDb == databases.end()) {
        throw Exception("Database " + dbName + " not found");
    }
    RemoveDatabase(dbName);
}

void Manager::ParseUpdate(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "DATABASE") {
        throw Exception("Expected DATABASE");
    }

    std::string oldDbName;
    iss >> oldDbName;
    if (oldDbName.empty()) {
        throw Exception("Expected old database name");
    }
    auto foundTarget = databases.find(oldDbName);
    if (foundTarget == databases.end()) {
        throw Exception("Database " + oldDbName + " not found");
    }

    std::string newDbName;
    iss >> newDbName;
    if (newDbName.empty()) {
        throw Exception("Expected new database name");
    }
    auto foundExisting = databases.find(newDbName);
    if (foundExisting != databases.end()) {
        throw Exception("Databases " + newDbName + " already exists");
    }

    UpdateDatabase(oldDbName, newDbName);
}

void Manager::ParseSelect(std::istringstream& iss)
{
    std::string cmd;
    iss >> cmd;
    cmd = Utils::ToUpperString(cmd);
    if (cmd != "DATABASE") {
        throw Exception("Expected DATABASE");
    }
    SelectDatabase();
}

void Manager::LoadFromFile()
{
    databases.clear();
    for (const auto& entry : std::filesystem::directory_iterator(storagePath)) {
        std::string dbName = entry.path().stem().string();
        databases.emplace(dbName, new Database(ToPath(dbName), dbName));
    }
}

void Manager::InsertDatabase(const std::string& dbName)
{
    databases.emplace(dbName, new Database(ToPath(dbName), dbName));
    std::cout << "Database " + dbName + " created" << std::endl;
}

void Manager::RemoveDatabase(const std::string& dbName)
{
    std::filesystem::remove(ToPath(dbName));
    delete databases[dbName];
    databases.erase(dbName);
    std::cout << "Database " + dbName + " dropped" << std::endl;
}

void Manager::UpdateDatabase(const std::string& oldDbName, const std::string& newDbName)
{
    std::filesystem::rename(ToPath(oldDbName), ToPath(newDbName));
    databases[oldDbName]->name = newDbName;
    auto copy = databases[oldDbName];
    databases.erase(oldDbName);
    databases.emplace(newDbName, copy);
    std::cout << "Database " + oldDbName + " renamed as " + newDbName << std::endl;
}

void Manager::SelectDatabase()
{
    int count = 0;
    std::cout << std::endl;
    std::cout << "DATABASE VIEW: " << std::endl;
    std::cout << std::endl;
    for (const auto& db : databases) {
        std::cout << std::right << std::setw(6) << ++count << " >> " << db.second->GetName() << std::endl;
    }
    if (count == 0) {
        std::cout << "       >> " << "No database yet" << std::endl;
    }
    std::cout << std::endl;
}