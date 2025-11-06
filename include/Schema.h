#pragma once
#include <fstream>
#include <map>
#include "Field.h"
#include "Exception.h"

class Schema
{
	friend class Table;
	friend class Record;
	friend std::ostream& operator<<(std::ostream& os, const Schema& schema);
	friend std::ofstream& operator<<(std::ofstream& ofs, const Schema& schema);
	friend std::ifstream& operator>>(std::ifstream& ifs, Schema& schema);

protected:
	std::map<std::string, Utils::DataType>fields;

};

std::ostream& operator<<(std::ostream& os, const Schema& schema);

std::ofstream& operator<<(std::ofstream& ofs, const Schema& schema);

std::ifstream& operator>>(std::ifstream& ifs, Schema& schema);