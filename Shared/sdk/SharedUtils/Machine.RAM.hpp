#pragma once

#include <Shared/sdk/SharedUtils/Windows.COM.hpp>
#include <Shared/sdk/SharedUtils/Misc.hpp>

namespace NightMTA::Shared::Machine {
    class RAMInfo {
    public:
        class RAMStick {
            SString m_name;
            SString m_caption;
            SString m_description;
            std::uint32_t m_speed;
            std::uint32_t m_clockSpeed;
            std::uint32_t m_minVoltage;
            std::uint32_t m_configuredVoltage;
            std::uint32_t m_maxVoltage;
            std::uint64_t m_capacity;
            SString m_manufacturer;
            SString m_partNumber;
            SString m_serial;
            SString m_bankName;
            SString m_deviceLocator;
        public:
            RAMStick (
                SString name,
                SString caption,
                SString description,
                std::uint32_t speed,
                std::uint32_t clockSpeed,
                std::uint32_t minVoltage,
                std::uint32_t configuredVoltage,
                std::uint32_t maxVoltage,
                std::uint64_t capacity,
                SString manufacturer,
                SString partNumber,
                SString serial,
                SString bankName,
                SString deviceLocator
            ) noexcept :
                m_name(name),
                m_caption(caption),
                m_description(description),
                m_speed(speed),
                m_clockSpeed(clockSpeed),
                m_minVoltage(minVoltage),
                m_configuredVoltage(configuredVoltage),
                m_maxVoltage(maxVoltage),
                m_capacity(capacity),
                m_manufacturer(manufacturer),
                m_partNumber(partNumber),
                m_serial(serial),
                m_bankName(bankName),
                m_deviceLocator(deviceLocator) {}

            const SString& GetName() const noexcept { return m_name; }
            const SString& GetCaption() const noexcept { return m_caption; }
            const SString& GetDescription() const noexcept { return m_description; }
            const std::uint32_t& GetSpeed() const noexcept { return m_speed; }
            const std::uint32_t& GetClockSpeed() const noexcept { return m_clockSpeed; }
            const std::uint32_t& GetMinVoltage() const noexcept { return m_minVoltage; }
            const std::uint32_t& GetConfiguredVoltage() const noexcept { return m_configuredVoltage; }
            const std::uint32_t& GetMaxVoltage() const noexcept { return m_maxVoltage; }
            const std::uint64_t& GetCapacity() const noexcept { return m_capacity; }
            const SString& GetManufacturer() const noexcept { return m_manufacturer; }
            const SString& GetPartNumber() const noexcept { return m_partNumber; }
            const SString& GetSerial() const noexcept { return m_serial; }
            const SString& GetBankName() const noexcept { return m_bankName; }
            const SString& GetDeviceLocator() const noexcept { return m_deviceLocator; }
        };

        RAMInfo(const std::vector<RAMStick>& sticks) noexcept
            : m_sticks(sticks) {}

        const std::vector<RAMStick>& GetSticks() const noexcept {
            return m_sticks;
        }
    protected:
        std::vector<RAMStick> m_sticks;
    };

    inline RAMInfo GetRAMInfo() noexcept {
        Windows::COM::COMWrapper com;
        com.Connect();
        auto results = com.Query("SELECT * FROM Win32_PhysicalMemory");
        std::vector<RAMInfo::RAMStick> sticks;
        for (auto& stick : results) {
            SString name = stick["Name"];
            SString caption = stick["Caption"];
            SString description = stick["Description"];
            std::uint32_t speed = Misc::stoui32(stick["Speed"]);
            std::uint32_t clockSpeed = Misc::stoui32(stick["ConfiguredClockSpeed"]);
            std::uint32_t minVoltage = Misc::stoui32(stick["MinVoltage"]);
            std::uint32_t configuredVoltage = Misc::stoui32(stick["ConfiguredVoltage"]);
            std::uint32_t maxVoltage = Misc::stoui32(stick["MaxVoltage"]);
            std::uint64_t capacity = Misc::stoui64(stick["Capacity"]);
            SString manufacturer = stick["Manufacturer"];
            SString partNumber = stick["PartNumber"];
            SString serial = stick["SerialNumber"];
            SString bankName = stick["BankLabel"];
            SString deviceLocator = stick["DeviceLocator"];

            sticks.emplace_back(
                name,
                caption,
                description,
                speed,
                clockSpeed,
                minVoltage,
                configuredVoltage,
                maxVoltage,
                capacity,
                manufacturer,
                partNumber,
                serial,
                bankName,
                deviceLocator
            );
        }

        return RAMInfo(sticks);
    }
}
