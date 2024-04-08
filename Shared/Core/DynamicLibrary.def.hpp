#include <Shared/sdk/SharedUtils/Defines.hpp>
#include <Shared/Core/DynamicLibrary.hpp>
#include <Shared/Core/WindowsMessages.hpp>
#include <cstdio>

namespace NightMTA::Shared::Core {
	DynamicLibrary::DynamicLibrary() noexcept {}
	DynamicLibrary::~DynamicLibrary() noexcept {
		this->Unload();
	}

	void DynamicLibrary::Unload() noexcept {
		if (!this->m_hModule) return;
#if MTA_WIN
		FreeLibrary(this->m_hModule);
#else
		dlclose(this->m_hModule);
#endif
		this->m_hModule = nullptr;
	}
	
	bool DynamicLibrary::IsLoaded() const noexcept {
		return this->m_hModule != nullptr;
	}

    DynamicLibrary::ProcAddr DynamicLibrary::GetProcAddr(const char* szProcName) noexcept {
        if (!this->m_hModule)
            return nullptr;
        
#if MTA_WIN
        auto pFunc = ::GetProcAddress(this->m_hModule, szProcName);
#else
        char* szError = nullptr;
        dlerror();

        auto pFunc = dlsym(this->m_hModule, szProcName);
        szError = dlerror();
        if (szError)
            return nullptr;
#endif
        return reinterpret_cast<ProcAddr>(pFunc);
    }

    bool DynamicLibrary::Load(const char* szFilename) noexcept {
        // Unload the previous library
        this->Unload();

        // Load the new library
#if MTA_WIN
        m_hModule = LoadLibraryExA(szFilename, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

        // Failed?
        if (!m_hModule) {
            WinMessage msg(GetLastError());

            // Display the error message and exit the process
            printf("Loading library %s failed (%s)\n", szFilename, msg.c_str());
        }
#else
        m_hModule = dlopen(szFilename, RTLD_NOW);

        // Output error if needed
        if (!m_hModule) {
            const char* szError = dlerror();
            fprintf(stderr, "Loading library %s failed", szFilename);
            if (szError)
                fprintf(stderr, " (%s)\n", szError);
        }
#endif

        // Return whether we succeeded or not
        return m_hModule != nullptr;
    }
}