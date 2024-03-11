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

    ValueTy ty() final {
        return ValueTy::Level;
    }
};

class Univ : public Value {
public:
    ValuePtr level;

    explicit Univ(ValuePtr& l) : level(l) {}

    ValueTy ty() final {
        return ValueTy::Univ;
    }
};

class UnivOmega : public Value {
public:
    UnivOmega() {}
    ValueTy ty() final {
        return ValueTy::UnivOmega;
    }
};
}