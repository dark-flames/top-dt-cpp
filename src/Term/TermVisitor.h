#pragma once

#include <Common/Visitor.h>
#include <Term/TermNode.h>
#include <Term/PiNode.h>
#include <Term/RefNode.h>
#include <Term/PiNode.h>
#include <memory>
#include <iostream>
#include <type_traits>

using namespace std;

namespace term {
template<typename R>
class TermVisitor : public Visitor<Term, R> {
public:
    virtual R visit(TermPtr &term) {
#define DEF_CASE(TYPE, METHOD) \
            if (term->ty() == TermTy::TYPE) { \
                auto ptr = specialize_term_ptr<TYPE>(term); \
                return this->visit_##METHOD(ptr); \
            }
        FOR_TERM_TYPES(DEF_CASE)
#undef DEF_CASE
        throw std::runtime_error("Unknown Term Ptr.");
    }

#define DEF_VISITOR_METHOD(TYPE, METHOD) \
    virtual R visit_##METHOD(NodePtr<TYPE>& node) { \
        TermPtr ptr = generalize_term_ptr(node); \
        return Visitor<Term, R>::visit(ptr); \
    }

    FOR_TERM_TYPES(DEF_VISITOR_METHOD)

#undef DEF_VISITOR_METHOD
};
}

