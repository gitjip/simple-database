#pragma once
#include <exception>
#include <string>

class Exception : public std::exception
{
public:
    explicit Exception(const std::string& message);
    const char* what() const noexcept override;
private:
    std::string message;
};