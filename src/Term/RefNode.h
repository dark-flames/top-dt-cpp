#pragma once

#include <Common/Visitor.h>
#include <Term/Nodes.h>
#include <Term/TermNode.h>

#include <memory>

namespace term {

using Idx = unsigned int;

class Var : public Term {
public:
    Idx i;

    explicit Var(Idx i) : i(i) {}

    virtual TermTy ty() override {
        return TermTy::Var;
    }

    static TermPtr make_term_ptr(Idx i) {
        return std::make_shared<Var>(i);
    }
};
}
