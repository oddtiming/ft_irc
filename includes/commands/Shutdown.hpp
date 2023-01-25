#ifndef SHUTDOWN_HPP
#define SHUTDOWN_HPP

#pragma once

/* System Includes */
#include <string>

/* Local Includes */
#include "Command.hpp"

class Shutdown : public Command
{
    public:
        /* Constructors & Destructor */
        Shutdown(Server* server);
        ~Shutdown();

        /* Public Member Functions */
        void                execute(const Message& msg);

    private:

};

#endif