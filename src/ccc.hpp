#pragma once

#include <iostream>

namespace ccc
{
    class CCConfig
    {
    public:
        explicit CCConfig(const int shift, std::string text)
            : m_shift{shift}
            , m_text{text} {};

        // Not copyable
        CCConfig(const CCConfig&) = delete;
        CCConfig& operator=(const CCConfig&) = delete;

        // Not movable
        CCConfig(CCConfig&&) = delete;
        CCConfig& operator=(CCConfig&&) = delete;

        virtual ~CCConfig() = default;

    private:
        int m_shift;
        std::string m_text;
    };

    class CaesarCipher
    {
    private:
    };
}  // namespace ccc
