#pragma once

namespace NightMTA::SharedUtil::Misc {
    class CriticalSection {
    public:
        CriticalSection();
        ~CriticalSection();
        void Lock();
        void Unlock();

    private:
        void* m_pCriticalSection;
    };
}