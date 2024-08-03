#pragma once
#include <exception>
#include <string>
#include <utility>
class Error : public std::exception {
public:
    explicit Error (const char* mesg) : message{ mesg } {
    }
    explicit Error (std::string mesg) : message{ std::move (mesg) } {
    }

    [[nodiscard]] const char* what () const noexcept override {
        return message.c_str ();
    }

private:
    std::string message;
};
