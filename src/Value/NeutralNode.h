#pragma once

#include <Common/Visitor.h>
#include <Common/Types.h>
#include <Value/Nodes.h>
#include <Value/ValueNode.h>

#include <memory>

namespace value {
class Var : public Value {
public:
    DBLevel l;
    ValuePtr spine;

    Var(DBLevel l, ValuePtr& spine) : l(l), spine(std::move(spine)) {}

    ValueTy ty() final {
        return ValueTy::Var;
    }

    ValuePtr copy() final {
        return make_value_ptr<Var>(this->l, this->spine->copy());
    }
};

class Spine : public Value {
public:
    virtual ValueTy ty() override {
        return ValueTy::Spine;
    }

    virtual bool empty() const {
        return true;
    }

    virtual ValuePtr next() {
        throw std::runtime_error("Impossible");
    }

    ValuePtr copy() override {
        return make_value_ptr<Spine>();
    }
};

class App : public Spine {
public:
    ValuePtr next;
    ValuePtr parameter;

    App(ValuePtr& next, ValuePtr& parameter) : next(std::move(next)), parameter(std::move(parameter)) {}

    ValueTy ty() final {
        return ValueTy::App;
    }

    ValuePtr copy() final {
        return make_value_ptr<App>(this->next->copy(), this->parameter->copy());
    }
};
}
