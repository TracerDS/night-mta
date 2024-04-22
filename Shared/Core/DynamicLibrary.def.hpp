#include <Shared/Core/DynamicLibrary.hpp>
#include <Shared/Core/WindowsMessages.hpp>
#include <cstdio>

namespace NightMTA::Shared::Core {
	inline DynamicLibrary::DynamicLibrary() noexcept {}
	inline DynamicLibrary::~DynamicLibrary() noexcept {
		this->Unload();
	}

	inline void DynamicLibrary::Unload() noexcept {
		if (!this->m_hModule) return;
#if MTA_WIN
		FreeLibrary(this->m_hModule);
#else
		dlclose(this->m_hModule);
#endif
		this->m_hModule = nullptr;
	}
	
	inline bool DynamicLibrary::IsLoaded() const noexcept {
		return this->m_hModule != nullptr;
	}

    inline DynamicLibrary::ProcAddr DynamicLibrary::GetProcAddr(const char* szProcName) noexcept {
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

    inline bool DynamicLibrary::Load(const char* szFilename) noexcept {
        // Unload the previous library
        this->Unload();

        // Load the new library
#if MTA_WIN
        m_hModule = LoadLibraryExA(szFilename, nullptr, LOAD_WITH_ALTERED_SEARCH_PATH);

        // Failed?
        if (!m_hModule) {
            return false;
        }
#else
        m_hModule = dlopen(szFilename, RTLD_NOW);

        // Output error if needed
        if (!m_hModule) {
            return false;
        }
#endif

        // Return whether we succeeded or not
        return m_hModule != nullptr;
    }
}