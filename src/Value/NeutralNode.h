#pragma once

#include <Common/Visitor.h>
#include <Value/Nodes.h>
#include <Value/ValueNode.h>

#include <memory>

namespace value {
class Var : public Value {
public:
    Lvl l;
    ValuePtr spine;

    Var(Lvl l, ValuePtr& spine) : l(l), spine(spine) {}

    ValueTy ty() final {
        return ValueTy::Var;
    }
};

class Spine : public Value {
public:
    virtual ValueTy ty() {
        return ValueTy::Spine;
    }

    virtual bool empty() const {
        return true;
    }

    virtual ValuePtr& next() {
        throw std::runtime_error("Impossible");
    }
};

class App : public Spine {
public:
    ValuePtr next;
    ValuePtr parameter;

    App(ValuePtr& next, ValuePtr& parameter) : next(next), parameter(parameter) {}

    ValueTy ty() override {
        return ValueTy::App;
    }
};
}
