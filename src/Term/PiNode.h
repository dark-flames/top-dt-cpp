#pragma once

#include <Common/Visitor.h>
#include <Term/Nodes.h>
#include <Term/TermNode.h>

#include <memory>

namespace term {
// lambda expression
class Lambda : public Term {
public:
    std::string name;
    TermPtr body;

    Lambda(std::string name, TermPtr &body) : name(name), body(body) {}

    TermTy ty() final {
        return TermTy::Lambda;
    }

    static TermPtr make_term_ptr(std::string n, TermPtr b) {
        return std::make_shared<Lambda>(n, b);
    }
};

// application
class App : public Term {
public:
    TermPtr fun;
    TermPtr param;

    App(TermPtr &fun, TermPtr &param) : fun(fun), param(param) {}

    TermTy ty() final {
        return TermTy::App;
    }

    static TermPtr make_term_ptr(TermPtr f, TermPtr p) {
        return std::make_shared<App>(f, p);
    }
};

//pi type
class Pi : public Term {
public:
    std::string name;
    TermPtr domain;
    TermPtr codomain;

    Pi(std::string &name, TermPtr &domain, TermPtr &codomain) : name(name), domain(domain), codomain(codomain) {}

    TermTy ty() final {
        return TermTy::Pi;
    }

    static TermPtr make_term_ptr(std::string n, TermPtr c, TermPtr d) {
        return std::make_shared<Pi>(n, c, d);
    }
};
}