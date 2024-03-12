#pragma once

#include <Common/Visitor.h>
#include <Term/Nodes.h>
#include <Term/TermNode.h>

#include <memory>

namespace term {

class Var : public Term {
public:
    Idx i;

    explicit Var(Idx i) : i(i) {}

    TermTy ty() final {
        return TermTy::Var;
    }

    TermPtr copy() override {
        return make_term_ptr<Var>(this->i);
    }
};
}
