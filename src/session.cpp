#include "session.hpp"
#include <optional>

namespace mailgram {

inline std::optional<std::string> extract_address(const std::string & text)
{
    const auto left_bracket = text.find('<');
    const auto right_bracket = text.find('>');

    if ((left_bracket == -1) or (right_bracket == -1)) {
        return {};
    }

    return {text.substr(left_bracket + 1, right_bracket - left_bracket - 1)};
}

bool Session::handle_line(const std::string & line)
{
    std::istringstream is{line};

    const auto extract_word = [&]() {
        std::string word;
        is >> word;
        return std::move(word);
    };

    const auto command = extract_word();

    switch (state) {
    case State::Connected:
        if (command != "EHLO") {
            std::cout << "Unexpected command " << command << "\n";
            return false;
        }

        write_ok();
        state = State::HelloSent;
        break;

    case State::HelloSent:
        if (command == "MAIL") {
            if (const auto from_opt = extract_address(extract_word()); from_opt) {
                from = *from_opt;
            } else {
                std::cout << "Bad formatting: " << line << '\n';
                return false;
            }

            std::cout << "Mail from: " << from << '\n';
            write_ok();
        } else if (command == "RCPT") {
            if (const auto to_opt = extract_address(extract_word()); to_opt) {
                to = *to_opt;
            } else {
                std::cout << "Bad formatting: " << line << '\n';
                return false;
            }

            std::cout << "Mail to: " << to << '\n';
            write_ok();
        } else if (command == "DATA") {
            std::cout << "Receiving body\n";
            state = State::Data;
            write_ok();
        } else {
            std::cout << "Unknown command: " << line << '\n';
            write_error();
            return false;
        }
        break;

    case State::Data:
        if (line[0] == '.') {
            std::cout << "Body Receivied:\n";
            std::cout << body << '\n';
            write_ok();
            state = State::BodyReceived;
        } else {
            body += line.substr(0, line.size() - 1);
            body += '\n';
        }
        break;

    case State::BodyReceived:
        write_ok();
        return false;

    default:
        std::abort();
    }
}

} // namespace mailgram
