/*****************************************************************************
 *
 *  PROJECT:     Multi Theft Auto v1.0
 *  LICENSE:     See LICENSE in the top level directory
 *  FILE:        core/CThreadCommandQueue.cpp
 *  PURPOSE:     Thread command queue class
 *
 *  Multi Theft Auto is available from http://www.multitheftauto.com/
 *
 *****************************************************************************/

#include <Shared/sdk/SharedUtils/Defines.hpp>

#ifdef MTA_WIN

#include <Server/Core/ThreadCommandQueue.hpp>
#include <Server/Core/ModManager.hpp>
//#include "CModManagerImpl.h"

namespace NightMTA::Server::Core {
    void ThreadCommandQueue::Add(const char* szCommand) noexcept {
        // Lock the critical section, add it, then unlock
        m_Critical.Lock();
        m_Commands.push_back(szCommand);
        m_Critical.Unlock();
    }

    bool ThreadCommandQueue::Process(ModManager* pModManager) noexcept
    {
        bool bRequestedQuit = false;

        // Lock the critical section
        m_Critical.Lock();

        // Got commands to process?
        while (m_Commands.size() > 0)
        {
            // Grab the string
            const std::string& str = m_Commands.front();

            // Check for the most important command: quit and exit.
            if (str == "quit" || str == "exit")
            {
                bRequestedQuit = true;
            }
            else
            {
                // Otherwise, pass the command to the mod's input handler
                if (pModManager)
                    pModManager->HandleInput(str.c_str());
            }

            // Remove it from the queue
            m_Commands.pop_front();
        }

        // Unlock the critical section
        m_Critical.Unlock();

        return bRequestedQuit;
    }
}

#endif
