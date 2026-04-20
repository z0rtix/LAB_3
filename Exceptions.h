#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <string>
#include <exception>


class PolynomialException: public std::exception {
    private:
        std::string message;
    
    public:
        explicit PolynomialException(const std::string &msg) : message(msg) {}
        const char* what() const noexcept override {
            return message.c_str();
        }
};


#endif