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

    Lambda(std::string& name, TermPtr& body) : name(name), body(std::move(body)) {}

    TermTy ty() final {
        return TermTy::Lambda;
    }

    TermPtr copy() final {
        return make_term_ptr<Lambda>(this->name, this->body->copy());
    }
};

class LLambda : public Term {
public:
    std::string name;
    TermPtr body;

    LLambda(std::string& name, TermPtr& body) : name(name), body(std::move(body)) {}

    TermTy ty() override final {
        return TermTy::LLambda;
    }

    TermPtr copy() override final {
        return make_term_ptr<LLambda>(this->name, this->body->copy());
    }
};

// application
class App : public Term {
public:
    TermPtr fun;
    TermPtr param;

    App(TermPtr& fun, TermPtr& param) : fun(std::move(fun)), param(std::move(param)) {}

    TermTy ty() override final {
        return TermTy::App;
    }

    TermPtr copy() override final {
        return make_term_ptr<App>(this->fun->copy(), this->param->copy());
    }
};

//pi type
class Pi : public Term {
public:
    std::string name;
    TermPtr domain;
    TermPtr codomain;

    Pi(std::string& name, TermPtr& domain, TermPtr& codomain) :
        name(name),
        domain(std::move(domain)),
        codomain(std::move(codomain)) {}

    TermTy ty() override final {
        return TermTy::Pi;
    }

    TermPtr copy() override final {
        return make_term_ptr<Pi>(
            this->name,
            this->domain->copy(),
            this->codomain->copy()
        );
    }
};

class LPi : public Term {
public:
    std::string name;
    TermPtr codomain;

    LPi(std::string& name, TermPtr& codomain) : name(name), codomain(std::move(codomain)) {}

    TermTy ty() override final {
        return TermTy::LPi;
    }

    TermPtr copy() override final {
        return make_term_ptr<LPi>(this->name, this->codomain->copy());
    }
};
}