#pragma once

#include <Common/Visitor.h>
#include <Syntax/Nodes.h>
#include <Syntax/SyntaxNode.h>

#include <memory>

namespace syntax {
// lambda expression
class Lambda : public Syntax {
public:
    std::string name;
    SyntaxPtr body;

    Lambda(std::string& name, SyntaxPtr& body) : name(name), body(std::move(body)) {}

    SyntaxTy ty() final {
        return SyntaxTy::Lambda;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<Lambda>(this->name, this->body->copy());
    }
};

// application
class App : public Syntax {
public:
    SyntaxPtr fun;
    SyntaxPtr param;

    App(SyntaxPtr& fun, SyntaxPtr& param) : fun(std::move(fun)), param(std::move(param)) {}

    SyntaxTy ty() final {
        return SyntaxTy::App;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<App>(this->fun->copy(), this->param->copy());
    }
};

//pi type
class Pi : public Syntax {
public:
    std::string name;
    SyntaxPtr domain;
    SyntaxPtr codomain;

    Pi(std::string& name, SyntaxPtr& domain, SyntaxPtr& codomain) :
        name(name),
        domain(std::move(domain)),
        codomain(std::move(codomain)) {}

    SyntaxTy ty() final {
        return SyntaxTy::Pi;
    }

    SyntaxPtr copy() final {
        return make_syn_ptr<Pi>(
            this->name,
            this->domain->copy(),
            this->codomain->copy()
        );
    }
};
}