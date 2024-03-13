#pragma once
#include <exception>
#include <string>

class Exception : public std::exception {
public:
    virtual const char * what () const throw ()
    {
        return "Exception";
    }
};

class UnimplementedException : public Exception {
public:
    std::string msg;
    explicit UnimplementedException(std::string& msg) : msg(msg) {}

    explicit UnimplementedException(const char* msg) : msg(msg) {}

    const char* what () const throw ()
    {
        return "UnimplementedException";
    }
};

class ImpossibleException : public Exception {
private:

public:
    std::string msg;
    ImpossibleException(std::string& msg) : msg(msg) {}

    const char * what () const throw ()
    {
        return "ImpossibleException";
    }
};