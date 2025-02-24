#pragma once

#include "../IrcBot.hpp"

class ABotCommand;

class Help : public ABotCommand
{
    public:
        Help();
        ~Help();
        void execute();
};