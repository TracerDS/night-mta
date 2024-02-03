#pragma once

#include <Shared/sdk/SharedUtils/Defines.hpp>

#ifdef MTA_WIN

#include <list>
#include <string>

#include <Shared/sdk/SharedUtils/std.hpp>
#include <Shared/sdk/SharedUtils/Misc.hpp>


namespace NightMTA::Server::Core {
    class ModManager;

    class ThreadCommandQueue : public SharedUtil::STD::Iterator<std::list<std::string>> {
    public:
        void Add(const char* szCommand) noexcept;
        bool Process(ModManager* pModManager) noexcept;
    private:
        std::list<std::string>            m_Commands;
        SharedUtil::Misc::CriticalSection m_Critical;
    };
}

#endif