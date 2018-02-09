#pragma once
#include <string>

namespace mailgram {

class WordExtractor {
public:
    WordExtractor(const std::string & str) : str(str) { set_position_to_next_word(); }

    std::string_view next_word()
    {
        if (position >= str.size()) {
            return {};
        }

        const auto word_start = position;

        for (; (position < str.size()) and (not std::isspace(str[position])); ++position) {
        }

        const auto word_length = position - word_start;

        set_position_to_next_word();

        return {&str[word_start], word_length};
    }

private:
    void set_position_to_next_word()
    {
        for (; (position < str.size()) and (std::isspace(str[position])); ++position) {
        }
    }

    const std::string & str;
    std::size_t position = 0;
};

inline std::string_view extract_address(const std::string_view & text)
{
    const auto left_bracket = text.find('<');
    const auto right_bracket = text.find('>');

    if ((left_bracket == text.npos) or (right_bracket == text.npos)) {
        return {};
    }

    return {&text[left_bracket + 1], right_bracket - left_bracket - 1};
}

} // namespace mailgram
