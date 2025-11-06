#include <iostream>
#include <string>
#include "StateMachine.h"
#include "Manager.h"
#include "Exception.h"

int main()
{
    std::cout << "**********************************************************************" << std::endl;
    std::cout << "** Simple Database Manager System v1.0.0" << std::endl;
    std::cout << "** Copyright (c) 2025 Jilin University Software Institute" << std::endl;
    std::cout << "**********************************************************************" << std::endl;

    StateMachine sm(new Manager("./db_storage"));

    while (!sm.IsQuit()) {
        std::cout << std::endl;
        sm.PrintEntityStack();
        std::string cmdLine;
        std::getline(std::cin, cmdLine);
        try {
            sm.ParseCommand(cmdLine);
        } catch (const Exception& e) {
            std::cout << "Error: " << e.what() << std::endl;
        }
    }
    std::cout << "System exited with everything saved" << std::endl;
    return 0;
}