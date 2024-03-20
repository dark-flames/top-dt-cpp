#pragma once

#include <exception>
#include <string>

class Exception : public std::exception {
public:
    virtual const char* what() const throw() {
        return "Exception";
    }
};

class UnimplementedException : public Exception {
public:
    std::string msg;

    explicit UnimplementedException(std::string& msg) : msg(msg) {}

    explicit UnimplementedException(const char* msg) : msg(msg) {}

    const char* what() const throw() {
        return "UnimplementedException";
    }
};

class ImpossibleException : public Exception {
public:
    std::string msg;

    ImpossibleException(std::string& msg) : msg(msg) {}

    ImpossibleException(const std::string& msg) : msg(msg) {}

    const char* what() const throw() {
        return "ImpossibleException";
    }
};

class ParseException : public Exception {

public:
    std::string msg;

    ParseException(std::string& msg) : msg(msg) {}

    ParseException(const std::string& msg) : msg(msg) {}

    const char* what() const throw() {
        return "ParseException";
    }
};

class CannotFormat : public Exception {
public:
    int tabs;

    CannotFormat(int tabs) : tabs(tabs) {}

    const char* what() const throw() {
        return "CannotFormat";
    }
};