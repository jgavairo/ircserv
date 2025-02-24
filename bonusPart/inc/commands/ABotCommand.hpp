#ifndef ABOTCOMMAND_HPP
#define ABOTCOMMAND_HPP

#include "../IrcBot.hpp"

class ABotCommand
{
    protected:
        ABotCommand();
    public:
        virtual ~ABotCommand();
        virtual void execute() = 0;
};


#endif