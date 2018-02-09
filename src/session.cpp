#include "session.hpp"
#include "text.hpp"
#include <optional>

namespace mailgram {

bool Session::handle_line(const std::string & line)
{
    WordExtractor word_extractor(line);
    const auto command = word_extractor.next_word();

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
            if (const auto address = extract_address(word_extractor.next_word()); not address.empty()) {
                from = address;
            } else {
                std::cout << "Bad formatting: " << line << '\n';
                return false;
            }

            std::cout << "Mail from: " << from << '\n';
            write_ok();
        } else if (command == "RCPT") {
            if (const auto address = extract_address(word_extractor.next_word()); not address.empty()) {
                to = address;
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

    return true;
}

} // namespace mailgram
