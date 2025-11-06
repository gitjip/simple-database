#pragma once
#include <iostream>
#include <stack>
#include "Entity.h"

class StateMachine
{
public:
    StateMachine(Entity* manager);
    void ParseCommand(const std::string& cmdLine);
    void PrintEntityStack() const;
    bool IsQuit() const;

private:
    void ParseExit();
    void ParseBack();
    void ParseEntity(const std::string& cmdLine);

    std::stack<Entity*> entityStack;
    bool isQuit;
};