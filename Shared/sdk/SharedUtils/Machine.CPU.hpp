#pragma once

#include <cstdint>
#include <intrin.h>
#include <bitset>
#include <array>

#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/Windows.COM.hpp>

namespace NightMTA::Shared::Machine {
    class CPUInfo {
    public:
        enum class Architecture {
            UNKNOWN,
            AMD64,
            ARM,
            ARM64,
            IA64,
            INTEL,
        };
        enum class ProcType {
            UNKNOWN,
            OEM,
            IOP,
            DUAL,
        };

        CPUInfo(
            const Architecture& architecture,
            const ProcType& procType,
            const SString& procID,
            const std::uint16_t& cores,
            const std::uint16_t& threads,
            const std::uint16_t& procRevision,
            const std::uint32_t& clockSpeed,
            const SString& socket,
            const SString& vendor,
            const SString& brand,
            const std::unordered_map<std::string, bool>& features
        ) noexcept :
            m_architecture(architecture),
            m_procType(procType),
            m_procID(procID),
            m_cores(cores),
            m_threads(threads),
            m_procRevision(procRevision),
            m_clockSpeed(clockSpeed),
            m_socket(socket),
            m_vendor(vendor),
            m_brand(brand),
            m_features(features)
        {}

        const Architecture& GetArchitecture() const noexcept { return m_architecture; }
        const ProcType& GetProcessorType() const noexcept { return m_procType; }
        const SString& GetProcessorID() const noexcept { return m_procID; }
        const std::uint16_t& GetCores() const noexcept { return m_cores; }
        const std::uint16_t& GetThreads() const noexcept { return m_threads; }
        const std::uint16_t& GetProcessorRevision() const noexcept { return m_procRevision; }
        const std::uint32_t& GetClockSpeed() const noexcept { return m_clockSpeed; }
        const SString& GetSocket() const noexcept { return m_socket; }
        const SString& GetProcessorVendorName() const noexcept { return m_vendor; }
        const SString& GetProcessorName() const noexcept { return m_brand; }
        const std::unordered_map<std::string, bool>& GetFeatures() const noexcept {
            return m_features;
        }

        constexpr SString GetArchitectureString() const noexcept {
            return CPUInfo::ArchitectureToString(GetArchitecture());
        }
        constexpr SString GetProcessorTypeString() const noexcept {
            return CPUInfo::ProcessorTypeToString(GetProcessorType());
        }

        static constexpr Architecture ValueToArchitecture(std::uint16_t value) noexcept {
            switch (value) {
                case PROCESSOR_ARCHITECTURE_AMD64:
                    return Architecture::AMD64;
                case PROCESSOR_ARCHITECTURE_ARM:
                    return Architecture::ARM;
                case PROCESSOR_ARCHITECTURE_ARM64:
                    return Architecture::ARM64;
                case PROCESSOR_ARCHITECTURE_IA64:
                    return Architecture::IA64;
                case PROCESSOR_ARCHITECTURE_INTEL:
                    return Architecture::INTEL;
                case PROCESSOR_ARCHITECTURE_UNKNOWN:
                default:
                    return Architecture::UNKNOWN;
            }
        }

        static constexpr SString ArchitectureToString(const Architecture& arch) noexcept {
            switch (arch) {
                case Architecture::AMD64:
                    return "AMD64";
                case Architecture::ARM:
                    return "ARM";
                case Architecture::ARM64:
                    return "ARM64";
                case Architecture::IA64:
                    return "IA64";
                case Architecture::INTEL:
                    return "INTEL";
                case Architecture::UNKNOWN:
                default:
                    return "UNKNOWN";
            }
        }
        static constexpr SString ProcessorTypeToString(const ProcType& type) noexcept {
            switch (type) {
                case ProcType::OEM:
                    return "OEM";
                case ProcType::IOP:
                    return "IOP";
                case ProcType::DUAL:
                    return "DUAL";
                case ProcType::UNKNOWN:
                default:
                    return "UNKNOWN";
            }
        }

    protected:
        Architecture m_architecture;
        ProcType m_procType;
        SString m_procID;
        std::uint16_t m_cores;
        std::uint16_t m_threads;
        std::uint16_t m_procRevision;
        std::uint32_t m_clockSpeed;
        SString m_socket;
        SString m_vendor;
        SString m_brand;
        std::unordered_map<std::string, bool> m_features;
    };

    inline CPUInfo GetCPUInfo() noexcept {
        SYSTEM_INFO sysInfo;
        GetSystemInfo(&sysInfo);

        // get vendor name
        std::vector<std::array<int, 4>> cpuData;
        std::vector<std::array<int, 4>> cpuDataExtended;

        std::array<int, 4> cpui;

        // Calling __cpuid with 0x0 as the function_id argument
        // gets the number of the highest valid function ID.
        __cpuid(cpui.data(), 0);
        auto functionIds = cpui[0];

        for (int i = 0; i <= functionIds; ++i) {
            __cpuidex(cpui.data(), i, 0);
            cpuData.push_back(cpui);
        }

        SString vendorName;

        {
            char vendor[0x20]{ 0 };
            *reinterpret_cast<int*>(vendor) = cpuData[0][1];
            *reinterpret_cast<int*>(vendor + 4) = cpuData[0][3];
            *reinterpret_cast<int*>(vendor + 8) = cpuData[0][2];

            vendorName = vendor;
        }

        CPUInfo::ProcType procType;
        std::unordered_map<std::string, bool> cpuFeatures;

        if (functionIds >= 1) {
            std::bitset<32> temp;

            temp = cpuData[1][0];
            switch ((temp[13] << 1) | temp[12]) {
                case 0b00:
                    procType = CPUInfo::ProcType::OEM;
                    break;
                case 0b01:
                    procType = CPUInfo::ProcType::IOP;
                    break;
                case 0b10:
                    procType = CPUInfo::ProcType::DUAL;
                    break;
                case 0b11:
                default:
                    procType = CPUInfo::ProcType::UNKNOWN;
                    break;
            }

            temp = cpuData[1][2];
            cpuFeatures["SSE3"] = temp[0];
            cpuFeatures["PCLMULQDQ"] = temp[1];
            cpuFeatures["DEBUGPEBS"] = temp[2];
            cpuFeatures["MONITOR"] = temp[3];
            cpuFeatures["CPLDEBUGSTORE"] = temp[4];
            cpuFeatures["VMX"] = temp[5];
            cpuFeatures["SMX"] = temp[6];
            cpuFeatures["EIST"] = temp[7];
            cpuFeatures["TM2"] = temp[8];
            cpuFeatures["SSSE3"] = temp[9];
            cpuFeatures["CNXT-ID"] = temp[10];
            cpuFeatures["SDBG"] = temp[11];
            cpuFeatures["FMA"] = temp[12];
            cpuFeatures["CMPXCHG16B"] = temp[13];
            cpuFeatures["xTPR"] = temp[14];
            cpuFeatures["PDCM"] = temp[15];
            //cpuFeatures["reserved"] = temp[16];
            cpuFeatures["PCID"] = temp[17];
            cpuFeatures["DCA"] = temp[18];
            cpuFeatures["SSE4.1"] = temp[19];
            cpuFeatures["SSE4.2"] = temp[20];
            cpuFeatures["x2APIC"] = temp[21];
            cpuFeatures["MOVBE"] = temp[22];
            cpuFeatures["POPCNT"] = temp[23];
            cpuFeatures["TSC-Deadline"] = temp[24];
            cpuFeatures["AES"] = temp[25];
            cpuFeatures["XSAVE"] = temp[26];
            cpuFeatures["OSXSAVE"] = temp[27];
            cpuFeatures["AVX"] = temp[28];
            cpuFeatures["F16C"] = temp[29];
            cpuFeatures["RDRAND"] = temp[30];
            cpuFeatures["Hypervisor"] = temp[31];

            temp = cpuData[1][3];
            cpuFeatures["FPU"] = temp[0];
            cpuFeatures["VME"] = temp[1];
            cpuFeatures["DEBUGEXT"] = temp[2];
            cpuFeatures["PSE"] = temp[3];
            cpuFeatures["TSC"] = temp[4];
            cpuFeatures["MSR"] = temp[5];
            cpuFeatures["PAE"] = temp[6];
            cpuFeatures["MCE"] = temp[7];
            cpuFeatures["CX8"] = temp[8];
            cpuFeatures["APIC"] = temp[9];
            //cpuFeatures["reserved"] = temp[10];
            cpuFeatures["SEP"] = temp[11];
            cpuFeatures["MTRR"] = temp[12];
            cpuFeatures["PGE"] = temp[13];
            cpuFeatures["MCA"] = temp[14];
            cpuFeatures["CMOV"] = temp[15];
            cpuFeatures["PAT"] = temp[16];
            cpuFeatures["PSE-36"] = temp[17];
            cpuFeatures["PSN"] = temp[18];
            cpuFeatures["CLFSH"] = temp[19];
            //cpuFeatures["reserved"] = temp[20];
            cpuFeatures["DEBUGGENERAL"] = temp[21];
            cpuFeatures["ACPI"] = temp[22];
            cpuFeatures["MMX"] = temp[23];
            cpuFeatures["FXSR"] = temp[24];
            cpuFeatures["SSE"] = temp[25];
            cpuFeatures["SSE2"] = temp[26];
            cpuFeatures["SELFSNOOP"] = temp[27];
            cpuFeatures["HTT"] = temp[28];
            cpuFeatures["THERMALMONITOR"] = temp[29];
            cpuFeatures["IA64"] = temp[30];
            cpuFeatures["PBE"] = temp[31];
        }

        if (functionIds >= 7) {
            std::bitset<32> temp;

            temp = cpuData[7][1];
            cpuFeatures["FSGSBASE"] = temp[0];
            cpuFeatures["IA32_TSC_ADJUST"] = temp[1];
            cpuFeatures["SGX"] = temp[2];
            cpuFeatures["BMI1"] = temp[3];
            cpuFeatures["HLE"] = temp[4];
            cpuFeatures["AVX2"] = temp[5];
            cpuFeatures["FDP_EXCPTN_ONLY"] = temp[6];
            cpuFeatures["SMEP"] = temp[7];
            cpuFeatures["BMI2"] = temp[8];
            cpuFeatures["ERMS"] = temp[9];
            cpuFeatures["INVPCID"] = temp[10];
            cpuFeatures["RTM"] = temp[11];
            cpuFeatures["PQM"] = temp[12];
            //cpuFeatures["reserved"] = temp[13];
            cpuFeatures["MPX"] = temp[14];
            cpuFeatures["PQE"] = temp[15];
            cpuFeatures["AVX512F"] = temp[16];
            cpuFeatures["AVX512DQ"] = temp[17];
            cpuFeatures["RDSEED"] = temp[18];
            cpuFeatures["ADX"] = temp[19];
            cpuFeatures["SMAP"] = temp[20];
            cpuFeatures["AVX512_IFMA"] = temp[21];
            cpuFeatures["PCOMMIT"] = temp[22];
            cpuFeatures["CLFLUSHOPT"] = temp[23];
            cpuFeatures["CLWB"] = temp[24];
            cpuFeatures["Intel"] = temp[25];
            cpuFeatures["AVX512PF"] = temp[26];
            cpuFeatures["AVX512ER"] = temp[27];
            cpuFeatures["AVX512CD"] = temp[28];
            cpuFeatures["SHA"] = temp[29];
            cpuFeatures["AVX512BW"] = temp[30];
            cpuFeatures["AVX512VL"] = temp[31];

            temp = cpuData[7][2];
            cpuFeatures["PREFETCHWT1"] = temp[0];
            cpuFeatures["AVX512_VBMI"] = temp[1];
            cpuFeatures["UMIP"] = temp[2];
            cpuFeatures["PKU"] = temp[3];
            cpuFeatures["OSPKE"] = temp[4];
            cpuFeatures["WAITPKG"] = temp[5];
            cpuFeatures["AVX512_VBMI2"] = temp[6];
            cpuFeatures["CET_SS"] = temp[7];
            cpuFeatures["GFNI"] = temp[8];
            cpuFeatures["VAES"] = temp[9];
            cpuFeatures["VPCLMULQDQ"] = temp[10];
            cpuFeatures["AVX512_VNNI"] = temp[11];
            cpuFeatures["AVX512_BITALG"] = temp[12];
            cpuFeatures["TME_EN"] = temp[13];
            cpuFeatures["AVX512_VPOPCNTDQ"] = temp[14];
            //cpuFeatures["reserved"] = temp[15];
            cpuFeatures["LA57"] = temp[16];
            cpuFeatures["MAWAU"] = temp[17];
            cpuFeatures["RDPID"] = temp[18];
            cpuFeatures["KL"] = temp[19];
            //cpuFeatures["reserved"] = temp[20];
            cpuFeatures["BUS_LOCK_DETECT"] = temp[21];
            //cpuFeatures["reserved"] = temp[22];
            cpuFeatures["CLDEMOTE"] = temp[23];
            cpuFeatures["MOVDIRI"] = temp[24];
            cpuFeatures["MOVDIR64B"] = temp[25];
            cpuFeatures["ENQCMD"] = temp[26];
            cpuFeatures["SGX_LC"] = temp[27];
            cpuFeatures["PKS"] = temp[28];
            //cpuFeatures["reserved"] = temp[29];
            cpuFeatures["AVX512_4VNNIW"] = temp[30];
            cpuFeatures["AVX512_4FMAPS"] = temp[31];

            temp = cpuData[7][3];
            // cpuFeatures["reserved"] = temp[0];
            // cpuFeatures["reserved"] = temp[1];
            cpuFeatures["AVX512_VP2INTERSECT"] = temp[2];
            cpuFeatures["SRBDS_CTRL"] = temp[3];
            cpuFeatures["MD_CLEAR"] = temp[4];
            cpuFeatures["RTM_ALWAYS_ABORT"] = temp[5];
            cpuFeatures["TSX_FORCE_ABORT"] = temp[6];
            cpuFeatures["SERIALIZE"] = temp[7];
            cpuFeatures["HYBRID"] = temp[8];
            cpuFeatures["TSXLDTRK"] = temp[9];
            cpuFeatures["PCONFIG"] = temp[10];
            cpuFeatures["LBR"] = temp[11];
            cpuFeatures["CET_IBT"] = temp[12];
            cpuFeatures["AMX_BF16"] = temp[13];
            cpuFeatures["AVX512_FP16"] = temp[14];
            cpuFeatures["AMX_TILE"] = temp[15];
            cpuFeatures["AMX_INT8"] = temp[16];
            //cpuFeatures["reserved"] = temp[17];
            //cpuFeatures["reserved"] = temp[18];
            cpuFeatures["IBRS_IBPB"] = temp[19];
            cpuFeatures["STIBP"] = temp[20];
            cpuFeatures["L1D_FLUSH"] = temp[21];
            cpuFeatures["IA32_ARCH_CAPABILITIES"] = temp[22];
            cpuFeatures["IA32_CORE_CAPABILITIES"] = temp[23];
            cpuFeatures["SSBD"] = temp[24];
        }

        __cpuid(cpui.data(), 0x80000000);
        auto functionIdsExtended = cpui[0];
        for (int i = 0x80000000; i <= functionIdsExtended; ++i) {
            __cpuidex(cpui.data(), i, 0);
            cpuDataExtended.push_back(cpui);
        }

        SString brandName;

        if(functionIdsExtended >= 0x80000004)
        {
            char brand[0x40]{ 0 };
            memcpy(brand, cpuDataExtended[2].data(), sizeof(cpui));
            memcpy(brand + 16, cpuDataExtended[3].data(), sizeof(cpui));
            memcpy(brand + 32, cpuDataExtended[4].data(), sizeof(cpui));

            brandName = brand;
        }

        auto architecture = CPUInfo::ValueToArchitecture(sysInfo.wProcessorArchitecture);

        Windows::COM::COMWrapper com;
        com.Connect();
        auto results = com.Query("SELECT * FROM Win32_Processor")[0];

        auto procID = results["ProcessorId"];

        auto cores = results["NumberOfCores"];
        if (cores.empty()) {
            DWORD length = 0;
            GetLogicalProcessorInformationEx(
                LOGICAL_PROCESSOR_RELATIONSHIP::RelationAll,
                nullptr, &length
            );
            auto procHeap = GetProcessHeap();
            auto buffer = static_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*>(
                HeapAlloc(procHeap, HEAP_ZERO_MEMORY, length)
            );

            if (GetLogicalProcessorInformationEx(
                LOGICAL_PROCESSOR_RELATIONSHIP::RelationAll,
                buffer, &length
            )) {
                DWORD offset = 0;

                std::uint16_t tempCores = 0;
                while (offset < length) {
                    auto info = reinterpret_cast<SYSTEM_LOGICAL_PROCESSOR_INFORMATION_EX*>(
                        (BYTE*) buffer + offset
                    );
                    if (!info) break;

                    if (info->Relationship == RelationProcessorCore)
                        tempCores++;

                    offset += info->Size;
                }
                cores = std::to_string(tempCores);
            }

            HeapFree(procHeap, 0, buffer);
        }
        auto threads = results["ThreadCount"];
        if (threads.empty())
            threads = std::to_string(sysInfo.dwNumberOfProcessors);

        auto clockSpeed = results["MaxClockSpeed"];
        auto socket = results["SocketDesignation"];        
        auto procRevision = static_cast<std::uint16_t>(sysInfo.wProcessorRevision);

        return CPUInfo(architecture, procType, procID,
            Misc::stoui16(cores), Misc::stoui16(threads), procRevision,
            Misc::stoui32(clockSpeed), socket,
            vendorName, brandName, cpuFeatures
        );
    }
}
