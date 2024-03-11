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
    Closure body;

    Lambda(Closure& body) : body(body) {}

    ValueTy ty() final {
        return ValueTy::Lambda;
    }
};

class LLambda : public Value {
public:
    Closure body;

    LLambda(Closure& body) : body(body) {}

    ValueTy ty() final {
        return ValueTy::LLambda;
    }
};

//pi typeb
class Pi : public Value {
public:
    ValuePtr domain;
    Closure codomain;

    Pi(ValuePtr & domain, Closure & codomain) : domain(domain), codomain(codomain) {}

    ValueTy ty() final {
        return ValueTy::Pi;
    }
};

class LPi : public Value {
public:
    Closure codomain;

    LPi(Closure & codomain) : codomain(codomain) {}

    ValueTy ty() final {
        return ValueTy::LPi;
    }
};
}