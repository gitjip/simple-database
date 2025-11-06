#pragma once
#include <filesystem>
#include <map>
#include "Database.h"
#include "Exception.h"

class Manager : public Entity
{
public:
    Manager(const std::filesystem::path& path);
    ~Manager();
    Utils::EntityType GetType() const override;
    std::string GetName() const override;
    Entity* ParseCommand(const std::string& cmdLine) override;
    Entity* ParseUse(std::istringstream& iss) override;
    void ParseInsert(std::istringstream& iss) override;
    void ParseRemove(std::istringstream& iss) override;
    void ParseUpdate(std::istringstream& iss) override;
    void ParseSelect(std::istringstream& iss) override;
    void LoadFromFile() override;

private:
    void InsertDatabase(const std::string& dbName);
    void RemoveDatabase(const std::string& dbName);
    void UpdateDatabase(const std::string& oldDbName, const std::string& newDbName);
    void SelectDatabase();

    std::map<std::string, Database*> databases;
};