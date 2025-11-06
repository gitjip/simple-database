#pragma once
#include <filesystem>
#include "Utils.h"

class Entity
{
public:
    Entity(const std::filesystem::path& path);

    virtual Utils::EntityType GetType() const = 0;
    virtual std::string GetName() const = 0;
    virtual Entity* ParseCommand(const std::string& cmdLine) = 0;
    virtual Entity* ParseUse(std::istringstream& iss) = 0;
    virtual void ParseInsert(std::istringstream& iss) = 0;
    virtual void ParseRemove(std::istringstream& iss) = 0;
    virtual void ParseUpdate(std::istringstream& iss) = 0;
    virtual void ParseSelect(std::istringstream& iss) = 0;
    virtual void LoadFromFile() = 0;

protected:
    std::filesystem::path ToPath(const std::string& stem, const std::string& extension = "") const;
    const std::filesystem::path storagePath;
};