#pragma once

#include <Common/Visitor.h>
#include <Term/TermNode.h>

#include <memory>

namespace term {
// lambda expression
class Lambda : Term {
public:
    char* name;
    TermPtr body;

    Lambda(char* name, TermPtr body) : name(name), body(body) {}
};

template<typename R>
class LambdaVisitor: Visitor<Lambda, R> {
protected:
public:
    R visit(std::shared_ptr<Lambda> target) override {
        return this->visitLambda(target);
    }
protected:
    virtual R visitLambda(std::shared_ptr<Lambda> target);
};

// application
class App : Term {
public:
    TermPtr fun;
    TermPtr param;

    App (TermPtr fun, TermPtr param) : fun(fun), param(param) {}
};

template<typename R>
class AppVisitor: Visitor<App, R> {
public:
    R visit(std::shared_ptr<App> target) override {
        return this->visitApp(target);
    }
protected:
    virtual R visitApp(std::shared_ptr<App> target);
};
//pi type
class Pi : Term {
public:
    char* name;
    TermPtr domain;
    TermPtr codomain;
    Pi (char* name, TermPtr domain, TermPtr codomain) : name(name), domain(domain), codomain(codomain) {}
};

template<typename R>
class PiVisitor: Visitor<Pi, R> {
public:
    R visit(std::shared_ptr<Pi> target) override {
        return this->visitPi(target);
    }
protected:
    virtual R visitPi(std::shared_ptr<Pi> target);
};
}