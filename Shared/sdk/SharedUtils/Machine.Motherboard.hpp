#pragma once

#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/SString.hpp>

namespace NightMTA::Shared::Machine {
    class MotherboardInfo {
    public:
        struct RawSMBIOSData {
            BYTE Used20CallingMethod;
            BYTE MajorVersion;
            BYTE MinorVersion;
            BYTE Revision;
            DWORD Length;
            BYTE SMBIOSTableData[0xBB2];
        };

        MotherboardInfo() noexcept {}

        MotherboardInfo(
            const SString& vendor,
            const SString& biosVersion,
            const SString& releaseDate,
            const SString& manufacturer,
            const SString& productName,
            const SString& version,
            const SString& serial,
            const SString& SKUNumber,
            const SString& family
        ) noexcept :
            m_vendor(vendor),
            m_biosVersion(biosVersion),
            m_releaseDate(releaseDate),
            m_manufacturer(manufacturer),
            m_productName(productName),
            m_version(version),
            m_serial(serial),
            m_SKUNumber(SKUNumber),
            m_family(family)
        {}

        const SString& GetVendor() const noexcept { return m_vendor; }
        const SString& GetBiosVersion() const noexcept { return m_biosVersion; }
        const SString& GetReleaseDate() const noexcept { return m_releaseDate; }
        const SString& GetManufacturer() const noexcept { return m_manufacturer; }
        const SString& GetProductName() const noexcept { return m_productName; }
        const SString& GetVersion() const noexcept { return m_version; }
        const SString& GetSerial() const noexcept { return m_serial; }
        const SString& GetSKUNumber() const noexcept { return m_SKUNumber; }
        const SString& GetFamily() const noexcept { return m_family; }
    protected:
        SString m_vendor;
        SString m_biosVersion;
        SString m_releaseDate;
        SString m_manufacturer;
        SString m_productName;
        SString m_version;
        SString m_serial;
        SString m_SKUNumber;
        SString m_family;
    };

    inline MotherboardInfo GetMotherboardInfo()  {
        auto smBiosDataSize = GetSystemFirmwareTable('RSMB', 0, nullptr, 0);

        auto procHeap = GetProcessHeap();
        auto smBiosData = static_cast<MotherboardInfo::RawSMBIOSData*>(
            HeapAlloc(procHeap, HEAP_ZERO_MEMORY, smBiosDataSize)
        );
        if (!smBiosData)
            return {};

        auto bytesWritten = GetSystemFirmwareTable('RSMB', 0, smBiosData, smBiosDataSize);

        SString vendorName;
        for (BYTE i = 0x1A; i < 0x33; i++)
            vendorName += smBiosData->SMBIOSTableData[i];

        SString biosVersion;
        for (BYTE i = 0x32; i < 0x37; i++)
            biosVersion += smBiosData->SMBIOSTableData[i];

        SString releaseDate;
        for (BYTE i = 0x37; i < 0x42; i++)
            releaseDate += smBiosData->SMBIOSTableData[i];

        SString manufacturer;
        for (BYTE i = 0x5F; i < 0x73; i++)
            manufacturer += smBiosData->SMBIOSTableData[i];

        SString productName;
        for (BYTE i = 0x73; i < 0x87; i++)
            productName += smBiosData->SMBIOSTableData[i];

        SString version;
        for (BYTE i = 0x87; i < 0x96; i++)
            version += smBiosData->SMBIOSTableData[i];

        SString serial;
        for (BYTE i = 0x96; i < 0xAB; i++)
            serial += smBiosData->SMBIOSTableData[i];

        SString SKUNumber;
        for (BYTE i = 0xAB; i < 0xAF; i++)
            SKUNumber += smBiosData->SMBIOSTableData[i];

        SString family;
        for (BYTE i = 0xAF; i < 0xC6; i++)
            family += smBiosData->SMBIOSTableData[i];

        HeapFree(procHeap, 0, smBiosData);

        return MotherboardInfo(
            vendorName, biosVersion, releaseDate, manufacturer,
            productName, version, serial, SKUNumber, family
        );
    }
}
