#pragma once

#include <Common/Visitor.h>
#include <Term/TermNode.h>
#include <Term/PiNode.h>
#include <Term/RefNode.h>
#include <Term/PiNode.h>
#include <Term/UnivNode.h>
#include <Term/Helper.h>
#include <memory>
#include <iostream>
#include <type_traits>

namespace term {
template<typename R>
class TermVisitor : public Visitor<Term, R> {
public:
    virtual R visit(Term& term) override {
        auto ptr = &term;
        switch (term.ty()) {
#define DEF_CASE(TYPE, METHOD) \
            case TermTy::TYPE:  \
                return this->visit_##METHOD(*static_cast<TYPE*>(ptr));
            FOR_TERM_TYPES(DEF_CASE)
#undef DEF_CASE
            default:
                throw std::runtime_error("Unknown Term Ptr.");
        }
    }

protected:
#define DEF_VISITOR_METHOD(TYPE, METHOD) \
    virtual R visit_##METHOD(TYPE& node) { \
        return Visitor<Term, R>::visit(node); \
    }

    FOR_TERM_TYPES(DEF_VISITOR_METHOD)

#undef DEF_VISITOR_METHOD
};
}

using term::TermVisitor;