#pragma once

namespace NightMTA::SharedUtil::STD {
    template <class Container>
    struct Iterator {
    protected:
        Container m_container;
    public:
        virtual constexpr typename Container::iterator begin() noexcept {
            return m_container.begin();
        }
        virtual constexpr typename Container::iterator end() noexcept {
            return m_container.end();
        }
        virtual constexpr const typename Container::const_iterator cbegin() const noexcept {
            return m_container.cbegin();
        }
        virtual constexpr const typename Container::const_iterator cend() const noexcept {
            return m_container.cend();
        }
    };
}