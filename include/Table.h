#pragma once
#include <filesystem>
#include <string>
#include <list>
#include <map>
#include "Entity.h"
#include "Record.h"

class Table :
    public Entity
{
	friend class Database;

public:
	Table(const std::filesystem::path& path, const std::string& name, const Schema& schema = Schema());
	~Table();
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
	void InsertRecord(const Record& record);
	void RemoveRecord(const std::string& fieldName, const Field& compared);
	void UpdateRecord(const std::string& mdfName, const Field& modified, const std::string& cmpName, const Field& compared);
	void SelectRecord(const std::string& fieldName, const Field& compared);
	void SelectAllRecords();
	static Schema ParseSchema(std::istringstream& iss);
	Record ParseRecord(std::istringstream& iss) const;

	void SaveToFile();
	void SaveSchema(std::ofstream& ofs) const;
	/* No line number */
	void SaveRecords(std::ofstream& ofs);
	void LoadSchema(std::ifstream& ifs);
	void LoadRecords(std::ifstream& ifs);

	void PrintDividerLine() const;

	std::string name;
	Schema schema;
	std::list<Record*>records;
	static const std::string extension;

};