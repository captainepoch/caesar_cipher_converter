#pragma once

#include <algorithm>
#include <cctype>
#include <iostream>
#include <memory>
#include <string>

namespace ccc
{
    enum CCConfigModeEnum
    {
        none,
        encode,
        decode
    };

    class CCConfig
    {
    public:
        explicit CCConfig(int shift, std::string text, CCConfigModeEnum mode)
            : m_shift{shift}
            , m_text{std::move(text)}
            , m_mode{mode}
        {
        }

        CCConfig(const CCConfig&) = delete;
        CCConfig& operator=(const CCConfig&) = delete;

        CCConfig(CCConfig&&) = default;
        CCConfig& operator=(CCConfig&&) = default;

        virtual ~CCConfig() = default;

        int get_shift_value() const
        {
            return m_shift;
        }
        void set_shift_value(int shift)
        {
            m_shift = shift;
        }

        const std::string& get_text() const
        {
            return m_text;
        }
        void set_text(std::string text)
        {
            m_text = std::move(text);
        }

        CCConfigModeEnum get_mode() const
        {
            return m_mode;
        }
        void set_mode(CCConfigModeEnum mode)
        {
            m_mode = mode;
        }

    private:
        int m_shift;
        std::string m_text;
        CCConfigModeEnum m_mode;
    };

    class CaesarCipher
    {
    public:
        explicit CaesarCipher(CCConfig* config)
            : m_config{config}
        {
        }

        CaesarCipher(const CaesarCipher&) = delete;
        CaesarCipher& operator=(const CaesarCipher&) = delete;

        CaesarCipher(CaesarCipher&&) = delete;
        CaesarCipher& operator=(CaesarCipher&&) = delete;

        virtual ~CaesarCipher() = default;

        std::string get_caesar_text() const
        {
            std::string transformed_text;
            transformed_text.resize(m_config->get_text().size());

            std::transform(
                m_config->get_text().begin(),
                m_config->get_text().end(),
                transformed_text.begin(),
                [this](unsigned char c)
                {
                    return get_caesar_shifted_char(c);
                });

            return transformed_text;
        }

    private:
        CCConfig* m_config;
        const unsigned char m_ascii_alphabet_lower[26] = {'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i',
                                                          'j', 'k', 'l', 'm', 'n', 'o', 'p', 'q', 'r',
                                                          's', 't', 'u', 'v', 'w', 'x', 'y', 'z'};
        const unsigned char m_ascii_alphabet_upper[26] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I',
                                                          'J', 'K', 'L', 'M', 'N', 'O', 'P', 'Q', 'R',
                                                          'S', 'T', 'U', 'V', 'W', 'X', 'Y', 'Z'};

        char get_caesar_shifted_char(unsigned char c) const
        {
            if (!std::isalpha(c))
            {
                return c;
            }

            bool is_upper_case = std::isupper(c);
            unsigned char base = is_upper_case ? 'A' : 'a';
            int idx = static_cast<int>(c - base);
            int shift = m_config->get_shift_value() % 26;
            if (m_config->get_mode() == encode)
            {
                idx = (idx + shift + 26) % 26;
            }
            else
            {
                idx = (idx - shift + 26) % 26;
            }

            return is_upper_case ? static_cast<char>(m_ascii_alphabet_upper[idx])
                                 : static_cast<char>(m_ascii_alphabet_lower[idx]);
        }
    };
}  // namespace ccc
