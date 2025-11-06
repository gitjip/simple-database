#include <iostream>
#include <sstream>
#include "StateMachine.h"
#include "Exception.h"

StateMachine::StateMachine(Entity* manager) :
    isQuit(false)
{
    entityStack.push(manager);
}

void StateMachine::ParseCommand(const std::string& cmdLine)
{
    if (cmdLine.empty()) {
        return;
    }
    std::string cmd = Utils::ToUpperString(cmdLine);
    if (cmd == "EXIT") {
        ParseExit();
    } else if (cmd == "BACK") {
        ParseBack();
    } else {
        ParseEntity(cmdLine);
    }
}

void StateMachine::PrintEntityStack() const
{
    std::stack<Entity*> copy = entityStack;
    std::vector<Entity*> entities;
    while (!copy.empty()) {
        entities.push_back(copy.top());
        copy.pop();
    }
    for (auto it = entities.rbegin(); it != entities.rend(); ++it) {
        std::cout << Utils::ToName((*it)->GetType()) << " " << (*it)->GetName() << " > ";
    }
    std::cout << std::endl;
}

bool StateMachine::IsQuit() const
{
    return isQuit;
}

void StateMachine::ParseExit()
{
    isQuit = true;
}

void StateMachine::ParseBack()
{
    if (entityStack.size() <= 1) {
        throw Exception("Already at top level");
    }
    entityStack.pop();
}

void StateMachine::ParseEntity(const std::string& cmdLine)
{
    Entity* current = entityStack.top();
    Entity* next = current->ParseCommand(cmdLine);
    if (next != nullptr) {
        entityStack.push(next);
    }
}