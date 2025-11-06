#pragma once
#include <map>
#include "Table.h"

class Database :
	public Entity
{
	friend class Manager;

public:
	Database(const std::filesystem::path& path, const std::string& name);
	~Database();
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
	void InsertTable(const std::string& tblName, const Schema& schema);
	void RemoveTable(const std::string& tblName);
	void UpdateTable(const std::string& oldTblName, const std::string& newTblName);
	void SelectTable();

	std::string name;
	std::map<std::string, Table*>tables;

};