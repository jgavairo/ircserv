#pragma once

#include "lib.hpp"

class Parser
{
    private:
    public:
        Parser();
        ~Parser();
        void parametersChecker(int argc, char** argv, int& _port);
        std::vector<std::string> splitByCRLF(const std::string& received);
};