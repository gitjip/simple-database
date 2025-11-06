#include <sstream>
#include "StateMachine.h"
#include "Exception.h"
#include "Utils.h"

StateMachine::StateMachine(Entity* manager) :
	isQuit(false)
{
	manager->LoadFromFile();
	entityStack.push(manager);
}

void StateMachine::ParseCommand(const std::string& cmdLine)
{
	std::istringstream iss(cmdLine);
	std::string cmd;
	iss >> cmd;
	cmd = Utils::ToUpperString(cmd);
	if (cmd.empty()) {
		return;
	} else if (cmd == "EXIT") {
		ParseExit();
	} else if (cmd == "BACK") {
		ParseBack();
	} else {
		ParseEntity(cmdLine);
	}
}

void StateMachine::PrintEntityStack() const
{
	std::stack<Entity*>copy = entityStack;
	std::stack<Entity*>reversed;
	while (!copy.empty()) {
		reversed.push(copy.top());
		copy.pop();
	}
	while (!reversed.empty()) {
		std::cout << Utils::ToName(reversed.top()->GetType()) << ":"
			<< reversed.top()->GetName() << (reversed.size() > 1 ? "\\" : ">");
		reversed.pop();
	}
}

bool StateMachine::IsQuit() const
{
	return isQuit;
}

void StateMachine::ParseExit()
{
	while (entityStack.size() > 1) {
		entityStack.pop();
	}
	Entity* manager = entityStack.top();
	delete manager;
	entityStack.pop();
	isQuit = true;
}

void StateMachine::ParseBack()
{
	if (entityStack.size() <= 1) {
		throw Exception("No parent directory");
	}
	entityStack.pop();
	Entity* parent = entityStack.top();
	parent->LoadFromFile();
}

void StateMachine::ParseEntity(const std::string& cmdLine)
{
	Entity* parent = entityStack.top();
	Entity* child = parent->ParseCommand(cmdLine);
	if (child != nullptr) {
		child->LoadFromFile();
		entityStack.push(child);
	}
}