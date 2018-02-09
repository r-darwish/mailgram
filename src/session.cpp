#include "session.hpp"
#include "text.hpp"
#include <optional>

namespace mailgram {

bool Session::handle_command(const std::string & line, const std::string_view & command,
                             WordExtractor & word_extractor)
{
    if (command == "MAIL") {
        if (const auto address = extract_address(word_extractor.next_word()); not address.empty()) {
            from = address;
        } else {
            std::cout << "Bad formatting: " << line << '\n';
            return false;
        }

        std::cout << "Mail from: " << from << '\n';
        write_ok();
        return true;

    } else if (command == "RCPT") {
        if (const auto address = extract_address(word_extractor.next_word()); not address.empty()) {
            to = address;
        } else {
            std::cout << "Bad formatting: " << line << '\n';
            return false;
        }

        std::cout << "Mail to: " << to << '\n';
        write_ok();
        return true;

    } else if (command == "DATA") {
        std::cout << "Receiving body\n";
        state = State::Data;
        write_ok();
        return true;

    } else {
        std::cout << "Unknown command: " << line << '\n';
        write_error();
        return false;
    }

    std::abort();
}

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
        return true;

    case State::HelloSent:
        return handle_command(line, command, word_extractor);

    case State::Data:
        if (line[0] == '.') {
            std::cout << body.str();
            write_ok();
            state = State::BodyReceived;
        } else {
            body << line << '\n';
        }
        return true;

    case State::BodyReceived:
        write_ok();
        return false;

    default:
        std::abort();
    }

    std::abort();
}

} // namespace mailgram
