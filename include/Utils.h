#pragma once
#include <string>
#include "Exception.h"

namespace Utils
{
    constexpr int COL_INTERVAL = 20;

    enum class EntityType
    {
        MANAGER,
        DATABASE,
        TABLE,
        RECORD,
        FIELD,
    };

    enum class DataType
    {
        INT,
        BOOL,
        FLOAT,
        STRING,
    };

    std::string ToName(EntityType entityType);
    std::string ToName(DataType dataType);
    DataType ToDataType(std::string name);
    std::string ToUpperString(std::string str);
}