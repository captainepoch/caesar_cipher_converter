#pragma once

#include <algorithm>
#include <iostream>

namespace ccc
{
    class CCConfig
    {
    public:
        explicit CCConfig(const int shift, std::string text)
            : m_shift{shift}
            , m_text{std::move(text)} {};

        // Not copyable
        CCConfig(const CCConfig&) = default;
        CCConfig& operator=(const CCConfig&) = delete;

        // Not movable
        CCConfig(CCConfig&&) = delete;
        CCConfig& operator=(CCConfig&&) = delete;

        virtual ~CCConfig() = default;

        const int get_shift_value()
        {
            return m_shift;
        }
        const std::string& get_text()
        {
            return m_text;
        }

    private:
        int m_shift;
        std::string m_text;
    };

    class CaesarCipher
    {
    public:
        explicit CaesarCipher(const CCConfig config)
            : m_config{config} {};

        // Not copyable
        CaesarCipher(const CaesarCipher&) = delete;
        CaesarCipher& operator=(const CaesarCipher&) = delete;

        // Not movable
        CaesarCipher(CaesarCipher&&) = delete;
        CaesarCipher& operator=(CaesarCipher&&) = delete;

        virtual ~CaesarCipher() = default;

        char get_caesar_shifted_char(char c)
        {
            if (c < 'a' || c > 'z')
            {
                return c;
            }

            int idx = c - 'a';
            idx = (idx + m_config.get_shift_value()) % 26;
            if (idx < 0)
            {
                idx += 26;
            }

            return m_ascii_alphabet[idx];
        }

        std::string encode()
        {
            std::string transformed_text;
            transformed_text.resize(m_config.get_text().size());

            std::transform(
                m_config.get_text().begin(),
                m_config.get_text().end(),
                transformed_text.begin(),
                [this](unsigned char c)
                {
                    return get_caesar_shifted_char(c);
                });

            return transformed_text;
        }

    private:
        CCConfig m_config;
        const unsigned char m_ascii_alphabet[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
                                                    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
    };
}  // namespace ccc
