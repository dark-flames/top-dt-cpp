#pragma once

#include <Common/Visitor.h>
#include <Term/TermNode.h>

#include <memory>

namespace term {

class Idx : Term {
public:
    unsigned int v;
    Idx(unsigned int v) : v(v) {}
};

template<typename R>
class IdxVisitor: Visitor<Idx, R> {
protected:
public:
    R visit(std::shared_ptr<Idx> target) override {
        return this->visitIdx(target);
    }
protected:
    virtual R visitIdx(std::shared_ptr<Idx> target);
};

}
