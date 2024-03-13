#pragma once

#include <Common/Visitor.h>
#include <Value/Nodes.h>
#include <Value/ValueNode.h>
#include <map>

namespace value {

using MetaNat = unsigned int;

class Level : public Value {
public:
    std::map<Lvl, MetaNat> m;
    MetaNat pure;

    Level(std::map<Lvl, MetaNat>& m, MetaNat pure) : m(m), pure(pure) {}

    explicit Level(MetaNat pure) : pure(pure) {}

    ValueTy ty() override final {
        return ValueTy::Level;
    }

    ValuePtr copy() override final {
        return make_value_ptr<Level>(this->m, this->pure);
    }
};

class Univ : public Value {
public:
    ValuePtr level;

    explicit Univ(ValuePtr& l) : level(std::move(l)) {}

    ValueTy ty() override final {
        return ValueTy::Univ;
    }

    ValuePtr copy() override final {
        return make_value_ptr<Univ>(this->level->copy());
    }
};

class UnivOmega : public Value {
public:
    UnivOmega() {}
    ValueTy ty() override final {
        return ValueTy::UnivOmega;
    }

    ValuePtr copy() override final {
        return make_value_ptr<UnivOmega>();
    }
};
}