#pragma once

#include <Common/Visitor.h>
#include <Value/Nodes.h>
#include <Value/ValueNode.h>
#include <Value/Closure.h>

#include <memory>

namespace value {
// lambda expression

class Lambda : public Value {
public:
    std::string name;
    Closure body;

    Lambda(std::string& name, Closure& body) : name(name), body(std::move(body)) {}

    ValueTy ty() final {
        return ValueTy::Lambda;
    }

    ValuePtr copy() final {
        return make_value_ptr<Lambda>(this->name, this->body);
    }
};

class LLambda : public Value {
public:
    std::string name;
    Closure body;

    LLambda(std::string& name, Closure& body) : name(name), body(std::move(body)) {}

    ValueTy ty() final {
        return ValueTy::LLambda;
    }

    ValuePtr copy() final {
        return make_value_ptr<LLambda>(this->name, this->body);
    }
};

//pi typeb
class Pi : public Value {
public:
    std::string name;
    ValuePtr domain;
    Closure codomain;

    Pi(
        std::string& name,
        ValuePtr& domain,
        Closure& codomain
    ) : name(name), domain(std::move(domain)), codomain(std::move(codomain)) {}

    ValueTy ty() final {
        return ValueTy::Pi;
    }

    ValuePtr copy() final {
        return make_value_ptr<Pi>(this->name, this->domain->copy(), this->codomain);
    }
};

class LPi : public Value {
public:
    std::string name;
    Closure codomain;

    LPi(std::string& name, Closure& codomain) : name(name), codomain(std::move(codomain)) {}

    ValueTy ty() final {
        return ValueTy::LPi;
    }

    ValuePtr copy() final {
        return make_value_ptr<LPi>(this->name, this->codomain);
    }
};
}