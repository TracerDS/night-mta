#pragma once

#include <iostream>
#include <unordered_map>

#include <Shobjidl.h>
#include <comdef.h>
#include <Wbemidl.h>

#include <Shared/sdk/SharedUtils/Windows.hpp>
#include <Shared/sdk/SharedUtils/UTF8.hpp>
#include <Shared/sdk/SharedUtils/Windows.COM.Map.hpp>

#pragma comment(lib, "wbemuuid.lib")

namespace NightMTA::Shared::Windows::COM {
    class COMWrapper {
        static inline std::int8_t gs_comObjects = 0;

        void SetLastError(const HRESULT& res) noexcept {
            m_lastErrorCode = res;
            m_lastError = UTF8::ToUTF8(_com_error(res).ErrorMessage());
        }
    public:
        using COMEntry = std::unordered_map<std::string, std::string>;

        COMWrapper() noexcept;
        ~COMWrapper() noexcept { Uninitialize(); }

        void Connect(const char* path = "ROOT\\CIMV2") noexcept;
        void Uninitialize() noexcept;

        std::vector<std::string> GetEnumeratedElements(const char* path) noexcept;
        std::vector<COMEntry> Query(const char* query) noexcept;

        constexpr bool IsInitialized() const noexcept { return m_isInitialized; }
        constexpr bool IsConnected() const noexcept { return m_isConnected; }
        constexpr std::string GetLastError() const noexcept { return m_lastError; }
        constexpr HRESULT GetLastErrorCode() const noexcept { return m_lastErrorCode; }
    protected:
        IEnumWbemClassObject* m_enumerator{ nullptr };
        IWbemLocator* m_locator{ nullptr };
        IWbemServices* m_services{ nullptr };

        bool m_isInitInitialized{ false };
        bool m_isInitialized{ false };
        bool m_isConnected{ false };

        std::string m_lastError{};
        HRESULT m_lastErrorCode{};
    };

    COMWrapper::COMWrapper() noexcept {
        HRESULT hres = CoInitializeEx(0, COINIT_MULTITHREADED);

        if (FAILED(hres)) {
            SetLastError(hres);
            return;
        }

        m_isInitInitialized = true;

        static bool gs_initializedSecurity = false;

        if (!gs_initializedSecurity) {
            hres = CoInitializeSecurity(
                nullptr,
                -1,
                nullptr,
                nullptr,
                RPC_C_AUTHN_LEVEL_DEFAULT,
                RPC_C_IMP_LEVEL_IMPERSONATE,
                nullptr,
                EOAC_NONE,
                nullptr
            );

            if (FAILED(hres)) {
                SetLastError(hres);
                Uninitialize();
                return;
            }
            gs_initializedSecurity = true;
        }
        gs_comObjects++;

        hres = CoCreateInstance(
            CLSID_WbemLocator,
            0,
            CLSCTX_INPROC_SERVER,
            IID_IWbemLocator,
            reinterpret_cast<LPVOID*>(&m_locator)
        );

        if (FAILED(hres)) {
            SetLastError(hres);
            Uninitialize();
            return;
        }

        m_isInitialized = true;
        SetLastError(S_OK);
    }

    void COMWrapper::Connect(const char* path) noexcept {
        SetLastError(S_OK);
        if (!m_isInitialized) return;

        assert(m_locator);

        HRESULT hres = m_locator->ConnectServer(
            _bstr_t(path),
            nullptr,
            nullptr,
            0,
            0,
            0,
            0,
            &m_services
        );

        if (FAILED(hres)) {
            SetLastError(hres);
            Uninitialize();
            return;
        }

        hres = CoSetProxyBlanket(
            m_services,
            RPC_C_AUTHN_WINNT,
            RPC_C_AUTHZ_NONE,
            nullptr,
            RPC_C_AUTHN_LEVEL_CALL,
            RPC_C_IMP_LEVEL_IMPERSONATE,
            nullptr,
            EOAC_NONE
        );

        if (FAILED(hres)) {
            SetLastError(hres);
            Uninitialize();
            return;
        }

        m_isConnected = true;
        SetLastError(S_OK);
    }

    void COMWrapper::Uninitialize() noexcept {
        if (!m_isInitInitialized) return;

        if (m_enumerator) {
            m_enumerator->Release();
            m_enumerator = nullptr;
        }
        if (m_locator) {
            m_locator->Release();
            m_locator = nullptr;
        }
        if (m_services) {
            m_services->Release();
            m_services = nullptr;
        }
        CoUninitialize();
        m_isInitialized = false;
        m_isInitInitialized = false;
    }

    std::vector<std::string> COMWrapper::GetEnumeratedElements(const char* path) noexcept {
        SetLastError(S_OK);

        assert(m_services);

        IWbemClassObject* pClass = nullptr;
        HRESULT hres = m_services->GetObject(
            bstr_t(path),
            0, nullptr, &pClass, nullptr
        );
        if (FAILED(hres)) {
            SetLastError(hres);
            return {};
        }

        SAFEARRAY* psaNames = nullptr;
        hres = pClass->GetNames(
            nullptr,
            WBEM_FLAG_ALWAYS | WBEM_FLAG_NONSYSTEM_ONLY,
            nullptr,
            &psaNames
        );

        if (FAILED(hres)) {
            SetLastError(hres);
            return {};
        }

        long lower, upper;
        BSTR propName = nullptr;
        SafeArrayGetLBound(psaNames, 1, &lower);
        SafeArrayGetUBound(psaNames, 1, &upper);

        std::vector<std::string> out;

        for (auto i = lower; i <= upper; i++) {
            // Get this property.
            hres = SafeArrayGetElement(
                psaNames,
                &i,
                &propName
            );
            auto size = wcslen(propName);
            std::wstring temp(size, 0);
            temp = propName;

            std::string utf8 = UTF8::ToUTF8(temp);

            out.push_back(utf8);
            SysFreeString(propName);
        }

        SafeArrayDestroy(psaNames);

        return out;
    }

    std::vector<COMWrapper::COMEntry> COMWrapper::Query(const char* query) noexcept {
        SetLastError(S_OK);
        assert(m_services);

        auto hres = m_services->ExecQuery(
            bstr_t("WQL"),
            bstr_t(query),
            WBEM_FLAG_FORWARD_ONLY | WBEM_FLAG_RETURN_IMMEDIATELY,
            nullptr,
            &m_enumerator
        );

        if (FAILED(hres)) {
            SetLastError(hres);
            return {};
        }

        IWbemClassObject* pclsObj = nullptr;
        ULONG uReturn = 0;

        std::vector<COMWrapper::COMEntry> values;

        while (m_enumerator) {
            COMWrapper::COMEntry field;
            HRESULT hr = m_enumerator->Next(WBEM_INFINITE, 1, &pclsObj, &uReturn);

            if (!uReturn || FAILED(hr))
                break;

            VARIANT vtProp{};
            CIMTYPE type;
            BSTR propertyName;
            SAFEARRAY* propertyNames;
            pclsObj->GetNames(nullptr,
                WBEM_FLAG_ALWAYS | WBEM_FLAG_NONSYSTEM_ONLY,
                nullptr,
                &propertyNames
            );

            LONG lLower, lUpper;
            SafeArrayGetLBound(propertyNames, 1, &lLower);
            SafeArrayGetUBound(propertyNames, 1, &lUpper);

            for (LONG i = lLower; i <= lUpper; i++) {
                SafeArrayGetElement(propertyNames, &i, &propertyName);
                hr = pclsObj->Get(propertyName, 0, &vtProp, &type, 0);

                if (SUCCEEDED(hr)) {
                    std::string name = UTF8::ToUTF8(propertyName);

                    // Print the value based on the type
                    switch (vtProp.vt) {
                        case VT_NULL:
                            break;
                        case VT_BSTR:
                        case VT_LPWSTR:
                            field[name] = UTF8::ToUTF8(vtProp.bstrVal);
                            break;
                        case VT_LPSTR:
                            field[name] = reinterpret_cast<LPCSTR>(vtProp.bstrVal);
                            break;
                        case VT_I2:
                        case VT_I4:
                        case VT_R4:
                        case VT_R8:
                        case VT_I1:
                        case VT_UI1:
                        case VT_UI2:
                        case VT_UI4:
                        case VT_I8:
                        case VT_UI8:
                        case VT_INT:
                        case VT_UINT:
                            field[name] = std::to_string(vtProp.intVal);
                            break;
                        case VT_BOOL:
                            field[name] = vtProp.boolVal ? "true" : "false";
                            break;
                        default:
                            field[name] = "[Unknown]";
                            break;
                    }
                }

                VariantClear(&vtProp);
                SysFreeString(propertyName);
            }

            SafeArrayDestroy(propertyNames);
            pclsObj->Release();

            values.push_back(field);
        }
        return values;
    }
}
