#pragma once

namespace ccc
{
    class CCConfig
    {
    public:
        explicit CCConfig(const long m_shift)
            : m_shift{m_shift} {};

        // Not copyable
        CCConfig(const CCConfig&) = delete;
        CCConfig& operator=(const CCConfig&) = delete;

        // Not movable
        CCConfig(CCConfig&&) = delete;
        CCConfig& operator=(CCConfig&&) = delete;

        virtual ~CCConfig() = default;

    private:
        long m_shift;
    };

    class CaesarCipher
    {
    private:
    };
}  // namespace ccc
